#include <Keypad.h>
#include <Servo.h>
#include <LiquidCrystal.h>

// Keypad setup (3x4)
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; // Connect to the row pins
byte colPins[COLS] = {3, 2, A5}; // Connect to the column pins
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Servo motor
Servo myServo;

// LCD (16x2)
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// Ultrasonic sensor
const int trigPin = A4;
const int echoPin = A3;

// Buzzer and LED
const int buzzerPin = A1;
const int ledPin = A0 ;

// Password system
String password = "1234";
String input = "";
bool isChangingPassword = false;
String newPassword = "";

// Timing
unsigned long doorOpenTime = 0;
bool doorOpen = false;

// Attempt control
int attempts = 0;
const int maxAttempts = 3;
bool lockedOut = false;
unsigned long lockoutStart = 0;
const unsigned long lockoutDuration = 10000; // 10 seconds lockout

// Welcome message timer
bool welcomeDisplayed = false;
unsigned long welcomeStartTime = 0;

void setup() {
  myServo.attach(A2);
  myServo.write(0); // Initially lock the door

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
}

void loop() {
  // Handle lockout after multiple failed attempts
  if (lockedOut) {
    if (millis() - lockoutStart >= lockoutDuration) {
      lockedOut = false;
      attempts = 0;
      digitalWrite(buzzerPin, LOW);
      lcd.clear();
      lcd.print("Try Again Now");
      delay(2000);
      lcd.clear();
    } else {
      digitalWrite(buzzerPin, HIGH);
      return;
    }
  }

  // Read distance from ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  if (distance < 30 && !welcomeDisplayed && input == "" && !isChangingPassword) {
    digitalWrite(ledPin, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("    Welcome!");
    
    welcomeDisplayed = true;
    welcomeStartTime = millis();
  }

  if (welcomeDisplayed && millis() - welcomeStartTime > 1000) {
    lcd.clear();
    welcomeDisplayed = false;
  }

  if (distance >= 30) {
    digitalWrite(ledPin, LOW);
  }

  // Auto-close the door after 5 seconds
  if (doorOpen && millis() - doorOpenTime >= 5000) {
    myServo.write(0);
    lcd.clear();
    lcd.print("Door Closed");
    delay(1000);
    lcd.clear();
    doorOpen = false;
  }

  // Handle keypad input
  char key = keypad.getKey();
  if (key) {
    if (key == '#') {
      if (isChangingPassword) {
        // Step 1: Check if the entered password is the correct current password
        if (newPassword == "" && input != "" && input != password) {
          lcd.clear();
          lcd.print("Wrong Old Pass");
          delay(1500);
          lcd.clear();
          input = "";
          isChangingPassword = false;
        }
        // Step 2: Correct current password entered, proceed to new password input
        else if (newPassword == "" && input == password) {
          lcd.clear();
          lcd.print("Enter New Pass");
          input = "";
          newPassword = "waiting"; // Marker to move to new password entry stage
        }
        // Step 3: Save new password, and ask for confirmation
        else if (newPassword == "waiting") {
          newPassword = input;
          input = "";
          lcd.clear();
          lcd.print("Confirm New Pass");
        }
        // Step 4: Compare confirmation with new password
        else {
          if (newPassword == input) {
            password = newPassword;
            lcd.clear();
            lcd.print("Pass Changed!");
          } else {
            lcd.clear();
            lcd.print("Mismatch! Retry");
          }
          // Reset all variables after process
          newPassword = "";
          input = "";
          isChangingPassword = false;
          delay(1500);
          lcd.clear();
        }
      } else {
        // Trigger password change mode with master code "0000"
        if (input == "0000") {
          lcd.clear();
          lcd.print("Enter Old Pass");
          input = "";
          isChangingPassword = true;
          newPassword = "";
        }
        // Normal access: correct password entered
        else if (input == password) {
          lcd.clear();
          lcd.print("Access Granted");
          myServo.write(90); // Open door
          doorOpenTime = millis();
          doorOpen = true;
          attempts = 0;
          input = ""; // Clear the input after successful login
          delay(5000);
        }
        // Incorrect password handling
        else {
          attempts++;
          lcd.clear();
          lcd.print("Wrong Password");
          if (attempts >= maxAttempts) {
            lcd.setCursor(0, 1);
            lcd.print("System Locked!");
            lockedOut = true;
            lockoutStart = millis();
          } else {
            for (int i = 0; i < 2; i++) {
              digitalWrite(buzzerPin, HIGH);
              delay(2000);
              digitalWrite(buzzerPin, LOW);
              delay(2000);
            }
          }
        }
        input = "";
      }
    }
    else if (key == '*') {
      input = "";
      lcd.clear();
      lcd.print("Cleared");
      delay(500);
      lcd.clear();
    } else {
      input += key;
      lcd.setCursor(0, 0);
      lcd.print(" Enter Password");
      lcd.setCursor(0, 1);
      lcd.print("Input: ");
      for (int i = 0; i < input.length(); i++) {
        lcd.print("*");
      }
      Serial.print("*");
    }
  }
}
