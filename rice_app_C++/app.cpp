#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <thread>

using namespace std;

class RiceCooker {
private:
    vector<string> functionalities;
    bool stopMaintain;

public:
    RiceCooker() : functionalities{"Cooking rice", "Keep warm", "Cooking schedule", "Quantity guide"}, stopMaintain(false) {}

    void showFunctionalities() {
        cout << "Rice Cooker Functionalities:" << endl;
        for (size_t i = 0; i < functionalities.size(); ++i) {
            cout << i + 1 << ". " << functionalities[i] << endl;
        }
    }

    int chooseOption() {
        int choice;
        cout << "Choose the menu:" << endl;
        cin >> choice;
        return choice;
    }

    void stopMaintainWarm() {
        stopMaintain = true;
    }

    void guideQuantities() {
        int persons;
        cout << "Enter the number of persons:" << endl;
        cin >> persons;

        while (true) {
            cout << "For " << persons << " person(s), you will need:" << endl;
            double quantityRice = persons * 100.0;
            double quantityWater = persons * 200.0;
            cout << "- Quantity of rice: " << quantityRice << " g" << endl;
            cout << "- Quantity of water: " << quantityWater << " ml" << endl;

            cout << "Press 1 to start cooking, 2 to change the number of persons, or Enter to quit." << endl;
            string choice;
            cin.ignore();
            getline(cin, choice);

            if (choice == "1") {
                cout << "Starting rice cooking." << endl;
                cookRice();
                return;
            } else if (choice == "2") {
                cout << "Changing the number of persons." << endl;
                cout << "Enter the number of persons:" << endl;
                cin >> persons;
            } else {
                cout << "Quit." << endl;
                return;
            }
        }
    }

    void keepWarm() {
        cout << "The keep warm functionality is activated." << endl;
        int secondsPassed = 0;

        while (!stopMaintain) {
            int minutes = secondsPassed / 60;
            int seconds = secondsPassed % 60;
            cout << "\rKeeping warm... " << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds;
            this_thread::sleep_for(chrono::seconds(1));
            secondsPassed++;
        }

        if (stopMaintain) {
            cout << endl << "Stopping keep warm." << endl;
        }
    }

    void keepRiceWarm() {
        int minutesToKeep;
        cout << "Enter the duration to keep warm in minutes:" << endl;
        cin >> minutesToKeep;

        if (minutesToKeep <= 0) {
            cout << "The keep warm duration is not valid." << endl;
            return;
        }

        cout << "Starting to keep warm for " << minutesToKeep << " minutes." << endl;
        stopMaintain = false;
        int secondsPassed = 0;

        while (!stopMaintain && secondsPassed < (minutesToKeep * 60)) {
            int minutes = secondsPassed / 60;
            int seconds = secondsPassed % 60;
            cout << "\rKeeping rice warm... " << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds << flush;
            this_thread::sleep_for(chrono::seconds(1));
            secondsPassed++;
        }

        if (!stopMaintain) {
            cout << endl << "Keeping rice warm is completed." << endl;
            stopMaintainWarm();
        }
    }

    void cookRice() {
        double waterLiters;
        int timer;

        cout << "Add the quantity of water:" << endl;
        cin >> waterLiters;

        cout << "Set the cooking timer (in minutes):" << endl;
        cin >> timer;

        if (waterLiters <= 0 || timer <= 0) {
            cout << "Cooking details are incomplete. Cooking cannot be started." << endl;
            return;
        }

        cout << "Starting rice cooking for " << fixed << setprecision(2) << waterLiters << " ml of water for " << timer << " minutes." << endl;

        for (int i = 1; i <= timer; ++i) {
            cout << "Cooking... " << i << " minute(s) passed" << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }

        cout << "Cooking is finished. Maintaining timer..." << endl;
        keepTimer(timer);
    }

    void scheduleCooking() {
        int hours, minutes, seconds;
        cout << "Set cooking time (HH:MM:SS):" << endl;
        scanf("%d:%d:%d", &hours, &minutes, &seconds);
        struct tm timeToCook = {0};
        timeToCook.tm_hour = hours;
        timeToCook.tm_min = minutes;
        timeToCook.tm_sec = seconds;

        chrono::system_clock::time_point scheduledTime = chrono::system_clock::from_time_t(mktime(&timeToCook));
        programCooking(scheduledTime);

        // Boucle pour attendre jusqu'à ce que le temps planifié soit atteint
        do {
            // Attendre une seconde avant de vérifier à nouveau
            this_thread::sleep_for(chrono::seconds(1));
        } while (chrono::system_clock::now() < scheduledTime);

        // Automatiquement commencer la fonction 1 après le temps planifié
        cout << "Automatically starting rice cooking after scheduling..." << endl;
        cookRice();
        stopMaintainWarm(); // Arrêter le maintien au chaud après la cuisson
    }

private:
    void keepTimer(int timer) {
        int secondsPassed = 0;

        while (!stopMaintain && secondsPassed < (timer * 60)) {
            int minutes = secondsPassed / 60;
            int seconds = secondsPassed % 60;
            cout << "\rKeeping warm... " << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds << flush;
            this_thread::sleep_for(chrono::seconds(1));
            secondsPassed++;
        }

        if (!stopMaintain) {
            cout << endl << "Keeping warm is completed." << endl;
        }
    }

    void programCooking(const chrono::system_clock::time_point& timeToCook) {
        time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        cout << "Cooking will start at " << put_time(localtime(&now), "%T") << "." << endl;

        if (timeToCook > chrono::system_clock::now()) {
            cout << "Starting cooking..." << endl;
            cookRice();
        }
    }
};

int main() {
    RiceCooker riceCooker;

    while (true) {
        riceCooker.showFunctionalities();

        int choice = riceCooker.chooseOption();

        switch (choice) {
            case 1:
                cout << "Starting rice cooking." << endl;
                riceCooker.cookRice();
                riceCooker.stopMaintainWarm();
                break;
            case 2:
                cout << "Keeping the cooked rice warm." << endl;
                riceCooker.keepRiceWarm();
                break;
            case 3:
                riceCooker.scheduleCooking();
                break;
            case 4:
                cout << "Enter the number of persons:" << endl;
                riceCooker.guideQuantities();
                break;
            default:
                cout << "Functionality not implemented." << endl;
        }

        cout << "Press Enter to return to the main menu or enter 'q' to quit." << endl;
        string quit;
        cin.ignore();
        getline(cin, quit);
        if (quit == "q") {
            break;
        }
    }

    cout << "Press Enter to quit." << endl;
    cin.ignore();
    return 0;
}
