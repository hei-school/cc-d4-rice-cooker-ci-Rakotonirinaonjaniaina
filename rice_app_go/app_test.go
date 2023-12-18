package main

import (
	"bytes"
	"io"
	"fmt"
	"os"
	"strings"
	"testing"
	
)

func TestNewRiceCooker(t *testing.T) {
	rc := NewRiceCooker()

	if rc == nil {
		t.Errorf("NewRiceCooker() returned nil, expected a valid RiceCooker instance")
	}
}

func TestCalculateQuantities(t *testing.T) {
	rc := NewRiceCooker()

	persons := 4
	expectedRiceQuantity := 400.0
	expectedWaterQuantity := 800.0

	riceQuantity, waterQuantity := rc.CalculateQuantities(persons)

	if riceQuantity != expectedRiceQuantity || waterQuantity != expectedWaterQuantity {
		t.Errorf("CalculateQuantities(%d) = (%f, %f), expected (%f, %f)",
			persons, riceQuantity, waterQuantity, expectedRiceQuantity, expectedWaterQuantity)
	}
}

func TestCookRice(t *testing.T) {
	rc := NewRiceCooker()

	// Mock user input for testing
	input := "300\n10\n"
	oldStdin := os.Stdin // Store the original os.Stdin so you can restore it later
	r, w, _ := os.Pipe()
	os.Stdin = r
	defer func() {
		os.Stdin = oldStdin // Restore original os.Stdin at the end of the test
	}()

	// Redirect stdout for testing
	oldStdout := os.Stdout
	os.Stdout = w
	defer func() {
		os.Stdout = oldStdout
	}()

	go func() {
		defer w.Close()
		fmt.Fprint(os.Stdout, input) // Simulate user input by writing directly to os.Stdout
		rc.CookRice()
	}()

	var output bytes.Buffer
	io.Copy(&output, r)

	expectedOutput := "Starting rice cooking for 300.00 ml of water for 10 minutes."
	if !strings.Contains(output.String(), expectedOutput) {
		t.Errorf("CookRice() output = %q, expected to contain %q", output.String(), expectedOutput)
	}
}
