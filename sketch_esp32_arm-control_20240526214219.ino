#include <BLEDevice.h> 
#include <BLEServer.h> 
#include <BLEUtils.h> 
#include <BLE2902.h> 
#include <String.h> 
#include <ESP32Servo.h> 
 
Servo myservo1;  // 定義servo對象，最多八個 
Servo myservo2;
Servo myservo3; 
int pos1 = 90;    // 定義servo轉動位置
int pos2 = 0;
int pos3 = 147;
int a =0;

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
uint8_t txValue = 0;
long lastMsg = 0;
String rxload="Test\n";

/*為BLE供應商定義指定的 UUID 編號*/
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" 
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

/*為BLE服務器編寫一個Callback函式來管理BLE的連接。*/
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

/*具有BLE功能的Callback函式。 
 *調用時，移動終端向 ESP32 發送數據時，會將其存儲到 reload 中。
 */
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      if (rxValue.length() > 0) {
        rxload="";
        for (int i = 0; i < rxValue.length(); i++){
          rxload +=(char)rxValue[i];
        }
      }
    }
};

/*創建BLE server流程：
 *1. Create a BLE Server
 *2. Create a BLE Service
 *3. Create a BLE Characteristic on the Service
 *4. Create a BLE Descriptor on the characteristic 
 *5. Start the service
 *6. Start advertising
 */
void setupBLE(String BLEName){
  const char *ble_name=BLEName.c_str();
  BLEDevice::init(ble_name);
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID); 
  pCharacteristic= pService->createCharacteristic(CHARACTERISTIC_UUID_TX,BLECharacteristic::PROPERTY_NOTIFY);
  pCharacteristic->addDescriptor(new BLE2902());
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX,BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks()); 
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void setup() {
  Serial.begin(9600);
  setupBLE("ESP32_BLE_Robert"); //初始化BLE並為其命名，藍牙顯示名稱可自行更改
  int pos1 = 90;    // 定義servo轉動位置
  int pos2 = 0;
  int pos3 = 147;
  int a = 0;
  myservo1.write(pos1);
      myservo1.attach(25);  // 設置servo控制腳位
    myservo2.write(pos2);
      myservo2.attach(26);
    myservo3.write(pos3);
      myservo3.attach(27);
  
}

/*當手機通過BLE藍牙向ESP32發送訊息時，會通過ESP32的序列視窗顯示出來； 
 *經由ESP32的序列視窗發送訊息後，會通過BLE藍牙發送到手機。
 */
void loop() {
  long now = millis();
  if (now - lastMsg > 100) {
    if (deviceConnected&&rxload.length()>0) {
        Serial.println(rxload);
        
    }
    
      //讀取字元
      if (rxload == "c" && pos1<=177 ){
        pos1 +=1;
        pos3 = 120-pos1-pos2;
        myservo3.write(pos3+a);
        myservo1.write(pos1);
        Serial.println("1前");
      }
      else if (rxload == "a" && pos1>1 ){
        pos1 -=1;
        pos3 = 120-pos1-pos2;
        myservo3.write(pos3+a);
        myservo1.write(pos1);
        
        Serial.println("1後");
      }
      else if (rxload == "b" && pos2<=177){
        pos2 +=1;
        pos3 = 120-pos1-pos2;
        myservo3.write(pos3+a);
        myservo2.write(pos2);
        
        Serial.println("2前");
      }
      else if (rxload == "d" && pos2>1 ){
        pos2 -=1;
        pos3 = 120-pos1-pos2;
        myservo3.write(pos3+a);
        myservo2.write(pos2);
        
        Serial.println("2後");
      }
      else if (rxload == "E" && (pos3+a)<=177){
        a +=3;
        myservo3.write(pos3+a);

        Serial.println("3前");
      }
      else if (rxload == "G" && (pos3+a)>1){
        a -=2;
        myservo3.write(pos3+a);
        Serial.println("3後");
      }
      else if (rxload == "H" ){
        
        a = 0;
        while(pos1<=115){
          pos1 +=1;
          myservo1.write(pos1);
          delay(30);
        }
        while(pos2>=0){
          pos2 -=1;
          myservo2.write(pos2);
          delay(30);
        }
        while(pos3>=0){
          pos3 -=1;
          myservo3.write(pos3);
          delay(30);
        }
        
        Serial.println("放球動作完成");
        delay(5000);
        
        while(pos1>=105){
          pos1 -=1;
          myservo1.write(pos1);
          delay(6);
        }
        while(pos2>=0){
          pos2 -=1;
          myservo2.write(pos2);
          delay(6);
        }
        while(pos3>=10){
          pos3 -=1;
          myservo3.write(pos3);
          delay(6);
        }
        Serial.println("初始化");

      }
      else if (rxload == "F" ){
        pos1 = 90;    // 定義servo轉動位置
        pos2 = 0;
        pos3 = 0;
        a = 0;
        myservo1.write(pos1);
        myservo2.write(pos2);
        myservo3.write(pos3);
      }
     
     
    
  delay(50);
  }
}