#include <stdio.h>

void calculateTax(int index, float taxRate, float incomeLimit, float higherTaxRate, float incomes[12], float *taxes, int *yearlyIncome);
void printTaxes(float incomes[12], float taxes[12]);

int main() {
	float incomes[12];
	float taxes[12];

	int yearlyIncome = 0;

	float taxRatePercent;
	printf("Enter tax rate: ");
	scanf("%f", &taxRatePercent);
	float taxRate = taxRatePercent / 100.0f;

	float incomeLimit;
	printf("Enter income limit: ");
	scanf("%f", &incomeLimit);

	float higherTaxRatePercent;
	printf("Enter tax rate for income over the limit: ");
	scanf("%f", &higherTaxRatePercent);
	float higherTaxRate = higherTaxRatePercent / 100.0f;

	for (int i = 0; i < 12; i++) {
		printf("Enter income for month %d: ", i + 1);
		scanf("%f", &incomes[i]);
		calculateTax(i, taxRate, incomeLimit, higherTaxRate, incomes, taxes, &yearlyIncome);
	}

	printTaxes(incomes, taxes);
}

void calculateTax(int index, float taxRate, float incomeLimit, float higherTaxRate, float incomes[12], float *taxes, int *yearlyIncome) {
	float incomeThisMonth = incomes[index];
	float tax = 0.0f;
	int prevYearlyIncome = *yearlyIncome;
	int newYearlyIncome = prevYearlyIncome + incomeThisMonth;

	if (prevYearlyIncome >= incomeLimit) {
		tax = incomeThisMonth * higherTaxRate;
	} else if (newYearlyIncome > incomeLimit) {
		float lowerPart = incomeLimit - prevYearlyIncome;
		float higherPart = incomeThisMonth - lowerPart;
		tax = lowerPart * taxRate + higherPart * higherTaxRate;
	} else {
		tax = incomeThisMonth * taxRate;
	}
	taxes[index] = tax;
	*yearlyIncome = newYearlyIncome;
}

void printTaxes(float incomes[12], float taxes[12]) {
	printf("%5s %10s %10s\n", "month", "income", "tax");
	for (int i = 0; i < 12; i++) {
		printf("%5d %10.2f %10.2f\n", i + 1, incomes[i], taxes[i]);
	}
}
