
from flask import Flask, request, jsonify, render_template
from werkzeug.utils import secure_filename
import os
import json
import uuid

app = Flask(__name__)

UPLOAD_FOLDER = "static/uploads"
DATA_FILE = "products.txt"
REPORT_FILE = "Sustainability_Report.txt"

os.makedirs(UPLOAD_FOLDER, exist_ok=True)

app.config["UPLOAD_FOLDER"] = UPLOAD_FOLDER
app.config["MAX_CONTENT_LENGTH"] = 10 * 1024 * 1024

products = []

# Load previously saved products
if os.path.exists(DATA_FILE):
    try:
        with open(DATA_FILE, "r", encoding="utf-8") as file:
            products = json.load(file)
    except (json.JSONDecodeError, OSError):
        products = []


# Material-based demonstration estimates
MATERIAL_DATA = {
    "paper": [20, 25, 30, 20, 90],
    "recycled paper": [15, 20, 25, 15, 95],
    "plastic": [75, 70, 60, 85, 25],
    "glass": [45, 65, 35, 30, 80],
    "metal": [65, 75, 40, 35, 85],
    "cotton": [50, 40, 80, 45, 70]
}


def calculate_impact(material):
    values = MATERIAL_DATA.get(
        material.lower().strip(),
        [50, 50, 50, 50, 50]
    )

    return {
        "carbon": values[0],
        "energy": values[1],
        "water": values[2],
        "waste": values[3],
        "recycle": values[4]
    }


def calculate_score(data):
    return round(
        (100 - data["carbon"]) * 0.25 +
        (100 - data["energy"]) * 0.20 +
        (100 - data["water"]) * 0.15 +
        (100 - data["waste"]) * 0.15 +
        data["recycle"] * 0.25,
        2
    )


def save_products():
    with open(DATA_FILE, "w", encoding="utf-8") as file:
        json.dump(products, file, indent=4)


# HOME PAGE
@app.route("/")
def home():
    return render_template("index.html")


# MODULE 1 AND MODULE 2
@app.route("/api/products", methods=["POST"])
def add_product():

    name = request.form.get("name", "").strip()
    category = request.form.get("category", "").strip()
    brand = request.form.get("brand", "").strip()
    material = request.form.get("material", "").strip()
    image = request.files.get("image")

    if not all([name, category, brand, material, image]):
        return jsonify({"error": "All fields and an image are required"}), 400

    if not image.filename or not image.mimetype.startswith("image/"):
        return jsonify({"error": "Please upload a valid image"}), 400

    filename = secure_filename(image.filename)

    if not filename:
        return jsonify({"error": "Invalid image filename"}), 400

    extension = os.path.splitext(filename)[1].lower()

    if extension not in [".jpg", ".jpeg", ".png", ".webp", ".gif"]:
        return jsonify({"error": "Unsupported image format"}), 400

    unique_name = str(uuid.uuid4()) + extension
    image_path = os.path.join(UPLOAD_FOLDER, unique_name)

    image.save(image_path)

    product = {
        "id": str(uuid.uuid4()),
        "name": name,
        "category": category,
        "brand": brand,
        "material": material,
        "image": "/" + image_path.replace("\\", "/")
    }

    # Automatic material-based assessment
    product.update(calculate_impact(material))
    product["score"] = calculate_score(product)

    products.append(product)
    save_products()

    return jsonify({
        "message": "Product uploaded successfully",
        "product": product
    }), 201


# GET ALL PRODUCTS
@app.route("/api/products", methods=["GET"])
def get_products():
    return jsonify(products)


# MODULE 3: COMPARE TWO PRODUCTS
@app.route("/api/compare", methods=["POST"])
def compare_products():

    data = request.get_json(silent=True) or {}

    id1 = data.get("product1")
    id2 = data.get("product2")

    p1 = next((p for p in products if p["id"] == id1), None)
    p2 = next((p for p in products if p["id"] == id2), None)

    if not p1 or not p2:
        return jsonify({"error": "Please select two valid products"}), 400

    if id1 == id2:
        return jsonify({"error": "Select two different products"}), 400

    if p1["score"] > p2["score"]:
        winner = p1["name"]
    elif p2["score"] > p1["score"]:
        winner = p2["name"]
    else:
        winner = "Both products have equal scores"

    return jsonify({
        "product1": p1,
        "product2": p2,
        "recommendation": winner
    })


# GENERATE REPORT
@app.route("/api/report", methods=["GET"])
def generate_report():

    lines = [
        "SUSTAINABLE PRODUCT SELECTION REPORT",
        "=" * 45,
        ""
    ]

    for p in products:
        lines.extend([
            "Product: " + p["name"],
            "Category: " + p["category"],
            "Brand: " + p["brand"],
            "Material: " + p["material"],
            "Carbon Impact: " + str(p["carbon"]),
            "Energy Impact: " + str(p["energy"]),
            "Water Impact: " + str(p["water"]),
            "Waste Impact: " + str(p["waste"]),
            "Recyclability: " + str(p["recycle"]) + "%",
            "Sustainability Score: " + str(p["score"]) + "%",
            "-" * 40,
            ""
        ])

    with open(REPORT_FILE, "w", encoding="utf-8") as file:
        file.write("\n".join(lines))

    return jsonify({
        "message": "Report generated successfully",
        "filename": REPORT_FILE
    })


if __name__ == "__main__":
    app.run(debug=True)
