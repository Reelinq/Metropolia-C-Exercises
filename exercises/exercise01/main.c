#include <stdio.h>

void computeTransportCosts(float busTicketPrice, float taxiTripPrice, float *userMoney);

int main() {
	float busTicketPrice;
	printf("Enter price of bus ticket: ");
	scanf("%f", &busTicketPrice);

	float taxiTripPrice;
	printf("Enter price of taxi: ");
	scanf("%f", &taxiTripPrice);

	float userMoney;
	printf("How much money you have: ");
	scanf("%f", &userMoney);

	printf("You have %.2f euros left. \n", userMoney);

	while (userMoney > busTicketPrice || userMoney > taxiTripPrice)
	{
		computeTransportCosts(busTicketPrice, taxiTripPrice, &userMoney);
		printf("You have %.2f euros left. \n", userMoney);
	}
	printf("You need to walk. Bye");
}

void computeTransportCosts(float busTicketPrice, float taxiTripPrice, float *userMoney) {
	int userChoice;
	printf("Do you want to take \n\t 1) bus (%.2f euros)\n\t 2) taxi (%.2f euros)\n Enter your selection: ", busTicketPrice, taxiTripPrice);
	scanf("%d", &userChoice);

	if (userChoice == 1) {
		printf("You chose bus.\n");
		if (*userMoney >= busTicketPrice) {
			*userMoney -= busTicketPrice;
		} else {
			printf("You don't have enough money for bus.\n");
		}
	} else if (userChoice == 2) {
		printf("You chose taxi.\n");
		if (*userMoney >= taxiTripPrice) {
			*userMoney -= taxiTripPrice;
		} else {
			printf("You don't have enough money for taxi.\n");
		}
	}
}
