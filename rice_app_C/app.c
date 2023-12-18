#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// RiceCooker structure
typedef struct {
    char *functionalities[4];
    int stopMaintain; // Flag to stop the keep warm functionality
} RiceCooker;

// RiceDetails structure
typedef struct {
    double waterLiters;
    int timer;
} RiceDetails;

// Function prototypes
RiceCooker *newRiceCooker();
void showFunctionalities(RiceCooker *rc);
int chooseOption(RiceCooker *rc);
void stopMaintainWarm(RiceCooker *rc);
void programCooking(RiceCooker *rc, struct tm *timeToCook);
void guideQuantities(RiceCooker *rc);
int chooseNumberOfPersons(RiceCooker *rc);
void keepWarm(RiceCooker *rc);
void keepRiceWarm(RiceCooker *rc);
void cookRice(RiceCooker *rc);

// Function to create a new RiceCooker instance
RiceCooker *newRiceCooker() {
    RiceCooker *rc = (RiceCooker *)malloc(sizeof(RiceCooker));
    if (rc != NULL) {
        rc->functionalities[0] = "Cooking rice";
        rc->functionalities[1] = "Keep warm";
        rc->functionalities[2] = "Cooking schedule";
        rc->functionalities[3] = "Quantity guide";
        rc->stopMaintain = 0;
    }
    return rc;
}

// Function to display RiceCooker functionalities
void showFunctionalities(RiceCooker *rc) {
    printf("Rice Cooker Functionalities:\n");
    for (int i = 0; i < 4; i++) {
        printf("%d. %s\n", i + 1, rc->functionalities[i]);
    }
}

// Function to choose an option from RiceCooker functionalities
int chooseOption(RiceCooker *rc) {
    int choice;
    printf("Choose the menu:\n");
    scanf("%d", &choice);
    return choice;
}

// Function to stop the keep warm functionality
void stopMaintainWarm(RiceCooker *rc) {
    rc->stopMaintain = 1; // Set flag to stop keeping warm
}

// Function to schedule cooking at a specific time
void programCooking(RiceCooker *rc, struct tm *timeToCook) {
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", timeToCook);
    printf("Cooking will start at %s.\n", timeStr);

    time_t now;
    time(&now);
    if (difftime(mktime(timeToCook), now) > 0) {
        printf("Starting cooking...\n");
        cookRice(rc);
    }
}

// Function to display quantities of rice and water needed based on the number of persons
void guideQuantities(RiceCooker *rc) {
    int persons = chooseNumberOfPersons(rc);
    while (1) {
        printf("For %d person(s), you will need:\n", persons);
        double quantityRice = persons * 100.0; // In grams
        double quantityWater = persons * 200.0; // In milliliters
        printf("- Quantity of rice: %.2f g\n", quantityRice);
        printf("- Quantity of water: %.2f ml\n", quantityWater);

        printf("Press 1 to start cooking, 2 to change the number of persons, or Enter to quit.\n");
        char choice[10];
        scanf("%s", choice);

        if (strcmp(choice, "1") == 0) {
            printf("Starting rice cooking.\n");
            cookRice(rc);
            return;
        } else if (strcmp(choice, "2") == 0) {
            printf("Changing the number of persons.\n");
            persons = chooseNumberOfPersons(rc);
        } else {
            printf("Quit.\n");
            return;
        }
    }
}

// Function to choose the number of persons for the quantity guide
int chooseNumberOfPersons(RiceCooker *rc) {
    int persons;
    printf("Enter the number of persons:\n");
    scanf("%d", &persons);
    return persons;
}

// Function to activate the keep warm functionality
void keepWarm(RiceCooker *rc) {
    printf("The keep warm functionality is activated.\n");
    int secondsPassed = 0;

    while (!rc->stopMaintain) {
        int minutes = secondsPassed / 60;
        int seconds = secondsPassed % 60;
        printf("\rKeeping warm... %02d:%02d", minutes, seconds);
        fflush(stdout);
        secondsPassed++;
        sleep(1);
    }

    if (rc->stopMaintain) {
        printf("\nStopping keep warm.\n");
    }
}

// Function to keep the cooked rice warm for a specific duration
void keepRiceWarm(RiceCooker *rc) {
    int minutesToKeep;
    printf("Enter the duration to keep warm in minutes:\n");
    scanf("%d", &minutesToKeep);

    if (minutesToKeep <= 0) {
        printf("The keep warm duration is not valid.\n");
        return;
    }

    printf("Starting to keep warm for %d minutes.\n", minutesToKeep);

    rc->stopMaintain = 0;
    int secondsPassed = 0;

    while (!rc->stopMaintain && secondsPassed < (minutesToKeep * 60)) {
        int minutes = secondsPassed / 60;
        int seconds = secondsPassed % 60;
        printf("\rKeeping rice warm... %02d:%02d", minutes, seconds);
        fflush(stdout);
        secondsPassed++;
        sleep(1);
    }

    if (!rc->stopMaintain) {
        printf("\nKeeping rice warm is completed.\n");
        stopMaintainWarm(rc);
    }
}

// Function to manage the rice cooking process
void cookRice(RiceCooker *rc) {
    RiceDetails details;
    details.waterLiters = 0;
    details.timer = 0;

    printf("Add the quantity of water:\n");
    scanf("%lf", &details.waterLiters);

    printf("Set the cooking timer (in minutes):\n");
    scanf("%d", &details.timer);

    if (details.waterLiters <= 0 || details.timer <= 0) {
        printf("Cooking details are incomplete. Cooking cannot be started.\n");
        return;
    }

    printf("Starting rice cooking for %.2f ml of water for %d minutes.\n", details.waterLiters, details.timer);

    int i;
    for (i = 1; i <= details.timer; i++) {
        printf("Cooking... %d minute(s) passed\n", i);
        sleep(1);
    }

    printf("Cooking is finished. Proceeding to keep warm functionality...\n");
    keepWarm(rc);
}

int main() {
    RiceCooker *riceCooker = newRiceCooker();

    while (1) {
        showFunctionalities(riceCooker);

        int choice = chooseOption(riceCooker);

        switch (choice) {
        case 1:
            printf("Starting rice cooking.\n");
            cookRice(riceCooker);
            stopMaintainWarm(riceCooker);
            break;
        case 2:
            printf("Keeping the cooked rice warm.\n");
            keepRiceWarm(riceCooker);
            break;
        case 3: {
            printf("Set cooking time (HH:mm:ss):\n");
            int hours, minutes, seconds;
            scanf("%d:%d:%d", &hours, &minutes, &seconds);
            struct tm timeToCook = {0};
            timeToCook.tm_hour = hours;
            timeToCook.tm_min = minutes;
            timeToCook.tm_sec = seconds;
            programCooking(riceCooker, &timeToCook);
            break;
        }
        case 4:
            printf("Enter the number of persons:\n");
            guideQuantities(riceCooker);
            break;
        default:
            printf("Functionality not implemented.\n");
        }

        printf("Press Enter to return to the main menu or enter 'q' to quit.\n");
        char quit[10];
        scanf("%s", quit);
        if (strcmp(quit, "q") == 0) {
            break;
        }
    }

    printf("Press Enter to quit.\n");
    getchar();
    return 0;
}
