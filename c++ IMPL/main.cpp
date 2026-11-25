
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <string>
using namespace std;

// Product structure
struct Product {
    string name, category, brand, material, image;
    double carbon, energy, water, waste, recyclable;
    double score;
};

vector<Product> products;

// MODULE 1: PRODUCT DATA MANAGEMENT

void addProduct() {
    Product p;

    cout << "\n--- ADD PRODUCT DETAILS ---\n";

    cin.ignore();
    cout << "Enter Product Name: ";
    getline(cin, p.name);

    cout << "Enter Category: ";
    getline(cin, p.category);

    cout << "Enter Brand: ";
    getline(cin, p.brand);

    cout << "Enter Material: ";
    getline(cin, p.material);

    cout << "Enter Product Image Path: ";
    getline(cin, p.image);

    // MODULE 2: ENVIRONMENTAL ASSESSMENT

    cout << "\n--- ENVIRONMENTAL DETAILS (0-100) ---\n";

    cout << "Carbon Footprint: ";
    cin >> p.carbon;

    cout << "Energy Consumption: ";
    cin >> p.energy;

    cout << "Water Usage: ";
    cin >> p.water;

    cout << "Waste Generation: ";
    cin >> p.waste;

    cout << "Recyclability Percentage: ";
    cin >> p.recyclable;

    // Calculate Sustainability Score

    p.score =
        (100 - p.carbon) * 0.25 +
        (100 - p.energy) * 0.20 +
        (100 - p.water) * 0.15 +
        (100 - p.waste) * 0.15 +
        p.recyclable * 0.25;

    products.push_back(p);

    cout << "\nProduct Added Successfully!\n";
}

// Save product details

void saveProducts() {
    ofstream file("products.txt");

    for (auto p : products) {
        file << p.name << "\n"
             << p.category << "\n"
             << p.brand << "\n"
             << p.material << "\n"
             << p.image << "\n"
             << p.carbon << "\n"
             << p.energy << "\n"
             << p.water << "\n"
             << p.waste << "\n"
             << p.recyclable << "\n"
             << p.score << "\n";
    }

    file.close();
}

// Display products

void displayProducts() {
    cout << "\n--- PRODUCT DETAILS ---\n";

    if (products.empty()) {
        cout << "No products available.\n";
        return;
    }

    for (int i = 0; i < products.size(); i++) {
        cout << "\nProduct " << i + 1 << endl;
        cout << "Name: " << products[i].name << endl;
        cout << "Category: " << products[i].category << endl;
        cout << "Brand: " << products[i].brand << endl;
        cout << "Material: " << products[i].material << endl;
        cout << "Image Path: " << products[i].image << endl;
    }
}

// MODULE 2: ENVIRONMENTAL IMPACT ASSESSMENT

void environmentalAssessment() {
    if (products.empty()) {
        cout << "\nAdd products first!\n";
        return;
    }

    cout << "\n--- ENVIRONMENTAL IMPACT REPORT ---\n";

    for (auto p : products) {
        cout << "\nProduct: " << p.name << endl;
        cout << "Carbon Footprint: " << p.carbon << endl;
        cout << "Energy Consumption: " << p.energy << endl;
        cout << "Water Usage: " << p.water << endl;
        cout << "Waste Generation: " << p.waste << endl;
        cout << "Recyclability: " << p.recyclable << "%" << endl;

        cout << fixed << setprecision(2);
        cout << "Sustainability Score: "
             << p.score << "/100\n";
    }
}

// MODULE 3: PRODUCT COMPARISON

void compareProducts() {
    if (products.size() < 2) {
        cout << "\nAdd at least two products to compare.\n";
        return;
    }

    cout << "\n--- PRODUCT COMPARISON ---\n";

    for (int i = 0; i < products.size(); i++) {
        cout << products[i].name
             << " : " << products[i].score
             << "/100\n";
    }

    auto best = max_element(
        products.begin(),
        products.end(),
        [](Product a, Product b) {
            return a.score < b.score;
        }
    );

    cout << "\nMost Sustainable Product: "
         << best->name << endl;

    cout << "Sustainability Score: "
         << best->score << "/100\n";
}

// PRODUCT RECOMMENDATION

void recommendProduct() {
    if (products.empty()) {
        cout << "\nNo products available.\n";
        return;
    }

    auto best = max_element(
        products.begin(),
        products.end(),
        [](Product a, Product b) {
            return a.score < b.score;
        }
    );

    cout << "\n--- SUSTAINABLE PRODUCT RECOMMENDATION ---\n";

    cout << "Recommended Product: "
         << best->name << endl;

    cout << "Material: "
         << best->material << endl;

    cout << "Sustainability Score: "
         << best->score << "/100\n";

    cout << "Reason: Highest sustainability score "
         << "among the entered products.\n";
}

// REPORT GENERATION

void generateReport() {
    ofstream report("Sustainability_Report.txt");

    report << "SUSTAINABLE PRODUCT SELECTION REPORT\n";
    report << "====================================\n\n";

    for (auto p : products) {
        report << "Product Name: " << p.name << "\n";
        report << "Category: " << p.category << "\n";
        report << "Brand: " << p.brand << "\n";
        report << "Material: " << p.material << "\n";
        report << "Carbon Footprint: " << p.carbon << "\n";
        report << "Energy Consumption: " << p.energy << "\n";
        report << "Water Usage: " << p.water << "\n";
        report << "Waste Generation: " << p.waste << "\n";
        report << "Recyclability: " << p.recyclable << "%\n";
        report << "Sustainability Score: " << p.score << "/100\n";
        report << "------------------------------------\n";
    }

    report.close();

    cout << "\nReport Generated Successfully!";
    cout << "\nFile Name: Sustainability_Report.txt\n";
}

// MAIN FUNCTION

int main() {
    int choice;

    do {
        cout << "\n====================================\n";
        cout << " SUSTAINABLE PRODUCT SELECTION SYSTEM\n";
        cout << "====================================\n";

        cout << "1. Add Product Details\n";
        cout << "2. Display Product Details\n";
        cout << "3. Environmental Impact Assessment\n";
        cout << "4. Compare Products\n";
        cout << "5. Recommend Sustainable Product\n";
        cout << "6. Save Product Data\n";
        cout << "7. Generate Report\n";
        cout << "8. Exit\n";

        cout << "\nEnter Your Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addProduct();
                break;

            case 2:
                displayProducts();
                break;

            case 3:
                environmentalAssessment();
                break;

            case 4:
                compareProducts();
                break;

            case 5:
                recommendProduct();
                break;

            case 6:
                saveProducts();
                cout << "\nProduct Data Saved Successfully!\n";
                break;

            case 7:
                generateReport();
                break;

            case 8:
                cout << "\nThank You for Using the System!\n";
                break;

            default:
                cout << "\nInvalid Choice! Try Again.\n";
        }

    } while (choice != 8);

    return 0;
}
