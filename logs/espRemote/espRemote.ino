#include <esp_now.h>
#include <WiFi.h>

String inputString = "";
bool stringComplete = false;

// This ESP32 Board MAC Address: { 0xEC, 0xDA, 0x3B, 0x51, 0xA5, 0x84 };

// Kavishka's esp MAC cc:7b:5c:36:23:94

// RECEIVER MAC Address
// change this according to the micromouse esp
uint8_t broadcastAddress[] = { 0xCC, 0x7B, 0x5C, 0x36, 0x23, 0x94 };

// Structure example to send data, Must match the receiver structure
typedef struct trasnmitData {
  float fwdKp;
  float fwdKd;
  float rotKp;
  float rotKd;
  float steeringKp;
  float steeringKd;
  int speed;
  int omega;
} transmitData;

// Create a struct_message called myData
transmitData command;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

typedef struct receiveData {
  float speed;
  float omega;
  bool front;
  bool left;
  bool right;
  float error;
  float feedback;
} receiveData;

receiveData received;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&received, incomingData, sizeof(received));
  Serial.print(received.speed);
  Serial.print(",");
  Serial.print(received.omega);
  Serial.print(",");
  Serial.print(received.front ? 1 : 0);
  Serial.print(",");
  Serial.print(received.left ? 1 : 0);
  Serial.print(",");
  Serial.print(received.right ? 1 : 0);
  Serial.print(",");
  Serial.print(received.error);
  Serial.print(",");
  Serial.println(received.feedback);
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Receiving code
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  command.fwdKp = 0.04;
  command.fwdKd = 1.3;
  command.rotKp = 1.3;
  command.rotKd = 1.2;
  command.speed = 250;
  command.omega = 0;
  command.steeringKp = 1.5;
  command.steeringKd = 2;
  // Send message via ESP-NOW
  delay(1000);
  esp_now_send(broadcastAddress, (uint8_t *)&command, sizeof(command));
}

void loop() {

  // receivedCode handling
  // if (stringComplete)
  // {
  //   serialEvent();
  //   char type = identifyType(inputString);

  //   if (type == 'i')
  //   {
  //     Serial.println(inputString.toInt());
  //     command.intCmd = inputString.toInt();
  //     command.floatCmd = 0.0;
  //     command.stringCmd = "";
  //   }
  //   else if (type == 'f')
  //   {
  //     Serial.println(inputString.toFloat());
  //     command.floatCmd = inputString.toFloat();
  //     command.intCmd = 0;
  //     command.stringCmd = "";
  //   }
  //   else if (type == 's')
  //   {
  //     Serial.println(command.stringCmd);
  //     command.stringCmd = inputString; // transmitting string is okay, but need to receive them as char arrays
  //     command.floatCmd = 0.0;
  //     command.intCmd = 0;
  //   }


  // inputString = "";
  // stringComplete = false;
  // }
}

// commands given using serial will be detected by this
// void serialEvent()
// {
//   while (Serial.available())
//   {
//     char inChar = (char)Serial.read();
//     if (inChar == '\n')
//     {
//       stringComplete = true;
//     }
//     else
//     {
//       inputString += inChar;
//     }
//   }
// }

// // used to catergarize and assign data to the struct according to the data type
// char identifyType(String input)
// {
//   input.trim(); // Remove leading/trailing whitespace

//   // Check for integer
//   bool isInt = true;
//   for (unsigned int i = 0; i < input.length(); i++)
//   {
//     if (i == 0 && input.charAt(i) == '-')
//       continue; // Allow negative numbers
//     if (!isDigit(input.charAt(i)))
//     {
//       isInt = false;
//       break;
//     }
//   }
//   if (isInt)
//   {
//     return 'i';
//   }

//   // Check for float
//   bool isFloat = true;
//   bool hasDecimal = false;
//   for (unsigned int i = 0; i < input.length(); i++)
//   {
//     if (i == 0 && input.charAt(i) == '-')
//       continue; // Allow negative numbers
//     if (input.charAt(i) == '.' && !hasDecimal)
//     {
//       hasDecimal = true;
//       continue;
//     }
//     if (!isDigit(input.charAt(i)))
//     {
//       isFloat = false;
//       break;
//     }
//   }
//   if (isFloat && hasDecimal)
//   {
//     return 'f';
//   }

//   // If not int, or float, assume it's a string
//   return 's';
// }