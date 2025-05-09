import numpy as np
import matplotlib.pyplot as plt

def polynomial_fit(x, y, degree):
    # Create the power matrix for x values
    X = np.zeros((degree + 1, degree + 1))
    for i in range(degree + 1):
        for j in range(degree + 1):
            X[i, j] = np.sum(np.power(x, i + j))
    
    Y = np.zeros(degree + 1)
    for i in range(degree + 1):
        Y[i] = np.sum(y * np.power(x, i))
    
    # Solve the system of equations
    coeffs = np.linalg.solve(X, Y)
    
    return coeffs

def predict(coeffs, x):

    return sum(coeff * (x ** i) for i, coeff in enumerate(coeffs))

def parse_data(file_path):
    try:
        with open(file_path, 'r') as file:
            next(file)
            
            years = []
            percentages = []
            populations = []
            
            for line in file:
                parts = line.strip().split(',')
                if len(parts) >= 3:
                    year = int(parts[0])
                    percentage = float(parts[1])
                    population = float(parts[2])
                    
                    years.append(year)
                    percentages.append(percentage)
                    populations.append(population)
            
            return np.array(years), np.array(percentages), np.array(populations)
    except Exception as e:
        print(f"Error reading data: {e}")
        return np.array([]), np.array([]), np.array([])

def print_equation(coeffs, variable="x"):
    eq = "y = "
    for i, coeff in enumerate(coeffs):
        if i == 0:
            eq += f"{coeff:.6f}"
        else:
            sign = "+" if coeff >= 0 else ""
            eq += f" {sign} {coeff:.6f}*{variable}^{i}"
    
    return eq

def main():
    file_path = "Data Tugas Pemrograman A.csv"
    years, percentages, populations = parse_data(file_path)
    
    degree_percentage = 3
    degree_population = 3
    
    coeffs_percentage = polynomial_fit(years, percentages, degree_percentage)
    coeffs_population = polynomial_fit(years, populations, degree_population)
    
    print("Internet Usage Growth - Polynomial Curve Fitting")
    print("-" * 50)
    print("\nPolynomial Equations:")
    print(f"a. Percentage of Internet Users: {print_equation(coeffs_percentage)}")
    print(f"b. Population Growth: {print_equation(coeffs_population)}")
    
    try:
        year_to_predict = int(input("\nEnter year to predict: "))
        
        predicted_percentage = predict(coeffs_percentage, year_to_predict)
        predicted_population = predict(coeffs_population, year_to_predict)
        
        predicted_percentage = max(0, min(100, predicted_percentage))
        predicted_population = max(0, predicted_population)
        
        print(f"Predicted percentage for year {year_to_predict}: {predicted_percentage:.6f}%")
        print(f"Predicted population for year {year_to_predict}: {predicted_population:.0f}")
    except ValueError:
        print("Invalid input. Please enter a valid year.")
    
    plt.figure(figsize=(12, 8))
    plt.subplot(2, 1, 1)
    plt.scatter(years, percentages, color='blue', label='Actual Data')
    
    x_curve = np.linspace(min(years), max(years), 100)
    y_curve = np.array([predict(coeffs_percentage, x) for x in x_curve])
    plt.plot(x_curve, y_curve, 'r-', label='Fitted Curve')
    
    plt.title('Percentage of Internet Users Over Time')
    plt.xlabel('Year')
    plt.ylabel('Percentage')
    plt.legend()
    plt.grid(True)
    
    plt.subplot(2, 1, 2)
    plt.scatter(years, populations, color='blue', label='Actual Data')
    
    y_curve = np.array([predict(coeffs_population, x) for x in x_curve])
    plt.plot(x_curve, y_curve, 'r-', label='Fitted Curve')
    
    plt.title('Population Growth Over Time')
    plt.xlabel('Year')
    plt.ylabel('Population')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.savefig('curve_fitting_results.png')
    plt.show()

if __name__ == "__main__":
    main()