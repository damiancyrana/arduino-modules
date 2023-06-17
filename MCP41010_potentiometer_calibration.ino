#include <SPI.h>

const uint8_t CS_PIN = 10; // CS pin for MCP41010
const uint8_t ANALOG_IN_PIN = A0;

const double R1 = 30000.00;
const double R2 = 7500.00;
const double Vref = 2.5; // Reference voltage from MCP1525-I/TO
const uint16_t NUM_SAMPLES = 1024; // Number of samples for averaging

void setup() {
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize the CS pin as an output and set it to HIGH
    pinMode(CS_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);

    // Initialize the SPI interface
    SPI.begin();

    // Set the reference voltage to external
    analogReference(EXTERNAL);

    // Iterate through all steps of the potentiometer (0-255)
    for (uint8_t step = 0; step <= 255; step++) {
        // Calculate theoretical resistance
        double resistance = (10000.00 / 256.00) * step; // 10 kΩ / 256 steps * current step

        // Set the potentiometer to the current step
        setPotentiometer(step);

        // Collect multiple samples
        long sum_adc_values = 0;
        for (uint16_t i = 0; i < NUM_SAMPLES; i++) {
            sum_adc_values += analogRead(ANALOG_IN_PIN);
            delayMicroseconds(500); // Short delay between samples
        }

        // Average the samples
        double average_adc_value = static_cast<double>(sum_adc_values) / NUM_SAMPLES;
        double adc_voltage = (average_adc_value * Vref) / 1023.00;
        double in_voltage = adc_voltage * (R1 + R2) / R2;

        // Display resistance and voltage values in the terminal
        Serial.print("Step: ");
        Serial.print(step);
        Serial.print(", Resistance: ");
        Serial.print(resistance, 3);
        Serial.print(" ohm, Voltage: ");
        Serial.print(in_voltage, 1);
        Serial.println(" V");

        // Wait 5 second before changing to the next step
        delay(5000);
    }
}

void loop() {
}

void setPotentiometer(uint8_t value) {
    // Activate MCP41010
    digitalWrite(CS_PIN, LOW);

    // Send command and value to MCP41010
    SPI.transfer(0x11); // Command to set potentiometer
    SPI.transfer(value); // Potentiometer step value (0-255)

    // Deactivate MCP41010
    digitalWrite(CS_PIN, HIGH);
}
