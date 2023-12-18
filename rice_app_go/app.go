package main

import (
	"fmt"
	"strings"
	"time"
)

// RiceCooker represents a rice cooker structure.
type RiceCooker struct {
	Functionalities []string
	stopMaintain    chan struct{} // Channel to stop the keep warm functionality
}

// RiceDetails contains rice cooking details.
type RiceDetails struct {
	WaterLiters float64
	Timer       int
}

// NewRiceCooker creates a new instance of RiceCooker.
func NewRiceCooker() *RiceCooker {
	return &RiceCooker{
		Functionalities: []string{
			"Cooking rice",
			"Keep warm",
			"Cooking schedule",
			"Quantity guide",
		},
	}
}

// ShowFunctionalities displays the functionalities of the RiceCooker.
func (rc *RiceCooker) ShowFunctionalities() {
	fmt.Println("Rice Cooker Functionalities:")
	for index, functionality := range rc.Functionalities {
		fmt.Printf("%d. %s\n", index+1, functionality)
	}
}

// ChooseOption allows choosing an option from the RiceCooker.
func (rc *RiceCooker) ChooseOption() int {
	var choice int
	fmt.Println("Choose the menu:")
	fmt.Scanln(&choice)
	return choice
}

// StopMaintainWarm stops the keep warm functionality.
func (rc *RiceCooker) StopMaintainWarm() {
	if rc.stopMaintain != nil {
		close(rc.stopMaintain) // Send a signal to stop keeping warm
	}
}

// ProgramCooking schedules cooking at a specific time.
func (rc *RiceCooker) ProgramCooking(timeToCook time.Time) {
	fmt.Printf("Cooking will start at %s.\n", timeToCook.Format("15:04:05"))
	select {
	case <-time.After(timeToCook.Sub(time.Now())): // Start cooking after the time difference between specified and current time
		fmt.Println("Starting cooking...")
		rc.CookRice()
	}
}

// GuideQuantities displays the quantities of rice and water needed based on the number of people.
func (rc *RiceCooker) GuideQuantities(persons int) {
	for {
		fmt.Printf("For %d person(s), you will need:\n", persons)
		quantityRice, quantityWater := rc.CalculateQuantities(persons)
		fmt.Printf("- Quantity of rice: %.2f g\n", quantityRice)
		fmt.Printf("- Quantity of water: %.2f ml\n", quantityWater)

		fmt.Println("Press 1 to start cooking, 2 to change the number of persons, or Enter to quit.")
		var choice string
		fmt.Scanln(&choice)

		switch strings.ToLower(choice) {
		case "1":
			fmt.Println("Starting rice cooking.")
			rc.CookRice()
			return
		case "2":
			fmt.Println("Changing the number of persons.")
			persons = rc.ChooseNumberOfPersons()
		default:
			fmt.Println("Quit.")
			return
		}
	}
}

// ChooseNumberOfPersons allows choosing the number of persons for the quantity guide.
func (rc *RiceCooker) ChooseNumberOfPersons() int {
	var persons int
	fmt.Println("Enter the number of persons:")
	fmt.Scanln(&persons)
	return persons
}

// KeepWarm activates the keep warm functionality.
func (rc *RiceCooker) KeepWarm() {
	fmt.Println("The keep warm functionality is activated.")
	var secondsPassed int

	go func() {
		for {
			select {
			case <-rc.stopMaintain:
				return
			default:
				minutes := secondsPassed / 60
				seconds := secondsPassed % 60
				fmt.Printf("\rKeeping warm... %02d:%02d", minutes, seconds)
				time.Sleep(1 * time.Second)
				secondsPassed++
			}
		}
	}()

	for {
		select {
		case <-rc.stopMaintain:
			fmt.Println("\nStopping keep warm.")
			return
		default:
			time.Sleep(1 * time.Second)
		}
	}
}

// KeepRiceWarm keeps the cooked rice warm for a specific duration.
func (rc *RiceCooker) KeepRiceWarm() {
	var minutesToKeep int
	fmt.Println("Enter the duration to keep warm in minutes:")
	fmt.Scanln(&minutesToKeep)

	if minutesToKeep <= 0 {
		fmt.Println("The keep warm duration is not valid.")
		return
	}

	fmt.Printf("Starting to keep warm for %d minutes.\n", minutesToKeep)

	done := make(chan bool)
	rc.stopMaintain = make(chan struct{})

	var secondsPassed int
	go func() {
		for {
			select {
			case <-rc.stopMaintain:
				return
			default:
				minutes := secondsPassed / 60
				seconds := secondsPassed % 60
				fmt.Printf("\rKeeping rice warm... %02d:%02d", minutes, seconds)
				time.Sleep(1 * time.Second)
				secondsPassed++
			}
		}
	}()

	go func() {
		<-time.After(time.Duration(minutesToKeep) * time.Minute)
		done <- true
	}()

	select {
	case <-done:
		fmt.Println("Keeping rice warm is completed.")
		rc.StopMaintainWarm()
	}
}

// CookRice manages the rice cooking process.
func (rc *RiceCooker) CookRice() {
	details := RiceDetails{
		WaterLiters: 0,
		Timer:       0,
	}

	fmt.Println("Add the quantity of water:")
	fmt.Scanln(&details.WaterLiters)

	fmt.Println("Set the cooking timer (in minutes):")
	fmt.Scanln(&details.Timer)

	if details.WaterLiters <= 0 || details.Timer <= 0 {
		fmt.Println("Cooking details are incomplete. Cooking cannot be started.")
		return
	}

	fmt.Printf("Starting rice cooking for %0.2f ml of water for %d minutes.\n", details.WaterLiters, details.Timer)

	done := make(chan bool)
	rc.stopMaintain = make(chan struct{})

	go func() {
		for i := 1; i <= details.Timer; i++ {
			time.Sleep(1 * time.Second)
			fmt.Printf("Cooking... %d minute(s) passed\n", i)
		}
		done <- true
	}()

	select {
	case <-done:
		fmt.Println("Cooking is finished. Proceeding to keep warm functionality...")
		rc.KeepWarm()
	case <-time.After(time.Duration(details.Timer) * time.Minute):
		fmt.Println("Automatic stop: Cooking is finished. Proceeding to keep warm functionality...")
		rc.KeepWarm()
	}
}

// CalculateQuantities calculates rice and water quantities based on the number of persons.
func (rc *RiceCooker) CalculateQuantities(persons int) (float64, float64) {
	const quantityRicePerPerson = 100.0  // In grams
	const quantityWaterPerPerson = 200.0 // In milliliters
	return float64(persons) * quantityRicePerPerson, float64(persons) * quantityWaterPerPerson
}

func main() {
	riceCooker := NewRiceCooker()

	for {
		riceCooker.ShowFunctionalities()

		choice := riceCooker.ChooseOption()

		switch choice {
		case 1:
			fmt.Println("Starting rice cooking.")
			riceCooker.CookRice()
			riceCooker.StopMaintainWarm()
		case 2:
			fmt.Println("Keeping the cooked rice warm.")
			riceCooker.KeepRiceWarm()
		case 3:
			fmt.Println("Set cooking time (HH:mm:ss):")
			var timeStr string
			fmt.Scanln(&timeStr)

			timeToCook, err := time.Parse("15:04:05", timeStr)
			if err != nil {
				fmt.Println("Invalid time format:", err)
				return
			}
			riceCooker.ProgramCooking(timeToCook)

		case 4:
			persons := riceCooker.ChooseNumberOfPersons()
			riceCooker.GuideQuantities(persons)
		default:
			fmt.Println("Functionality not implemented.")
		}

		fmt.Println("Press Enter to return to the main menu or enter 'q' to quit.")
		var quit string
		fmt.Scanln(&quit)
		if strings.ToLower(quit) == "q" {
			break
		}
	}

	fmt.Println("Press Enter to quit.")
	fmt.Scanln()
}





//linter : https://sparkbox.com/foundry/go_vet_gofmt_golint_to_code_check_in_Go