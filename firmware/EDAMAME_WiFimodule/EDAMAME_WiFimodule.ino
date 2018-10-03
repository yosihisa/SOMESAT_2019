/*
2017noshiro_zunda_UDPsend
by yuuitirou528

アクセスポイントと自分のIPアドレスと設定すること
IPアドレスは重なってはいけない。

PCの通信ソフトの方を後に立ち上げるとシリアル通信の受信バッファ128バイトが送信されるので
PC通信ソフト→CanSat（通信モジュール）の順に電源を入れること
始めに以前に取得している受信バッファの内容が出ても困らなければ後に電源を入れても良い（注意）

*/

#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include <WiFiUdp.h>

String str = "NoData";    //文字列受信の為の変数
String str0 = "";   //初期化のための変数

// 無線LANアクセスポイントの設定
const char* ssid     = "EDAMAME";
const char* password = "edamame1234";

// UDPオブジェクト
static WiFiUDP udp;

// ポート番号
#define LOCAL_PORT  50003  // 自分のポート番号
#define REMOTE_PORT 50003  // 相手のポート番号

// IPアドレス
IPAddress localIP;  // 自分のIPアドレス
IPAddress remoteIP; // 相手のIPアドレス

/*
static const char *udpReturnAddr = "192.168.11.2";
static const int udpReturnPort = 80;
*/

void setup() {
  Serial.begin(115200);   //シリアル通信の通信速度
  pinMode(16, OUTPUT);  //シルクACTのLED　使用していない
  pinMode(5, OUTPUT);   //シルクLEDのLED　　アクセスランプ用

  delay(100);

  // APの設定
  WiFi.mode(WIFI_AP);

  /*
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  */

  WiFi.begin(ssid, password);

  // Solid IP http://ashiyu.cocolog-nifty.com/blog/2015/08/indexhtml-8dfa.html

  //ネットワーク設定 自分のIPアドレスを設定する
  WiFi.config(IPAddress(192, 168, 43, 103), WiFi.gatewayIP(), WiFi.subnetMask());
  /*
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println(WiFi.localIP());
  */
  localIP = WiFi.softAPIP();

  // UDPの開始
  udp.begin(LOCAL_PORT);
}

void loop() {
  
  char packetBuffer[256];
  char data[512];


  // パケット受信があればデータ取得
  int packetSize = udp.parsePacket();
  if (packetSize) {
    digitalWrite(5, HIGH);   //アクセスランプ点灯　シルクはLED
    int len = udp.read(packetBuffer, packetSize);
    if (len > 0) packetBuffer[len] = '\0';
     
    // 相手のIPアドレス取得
    remoteIP = udp.remoteIP();
    /*
    Serial.print(remoteIP);
    Serial.print(" / ");
    Serial.println(packetBuffer);   //受信パケット内容をシリアルに送る
    */
    
    //シリアルからデータ（文字列）受信
    //受信バッファは128バイトなので注意
    if (Serial.available() > 0)
    {
      str = str0; //文字列変数の中身を初期化
      str = Serial.readStringUntil('\n');   //終了文字は\n
    }

    //受信電界強度測定
    long rssi = WiFi.RSSI();
    //Serial.print("RSSI:");
    //Serial.println(rssi);

    //受信文字列データを配列に変換
    String tmp = ".";
    tmp = rssi + tmp;
    str = tmp + str;
    str.toCharArray(data, 256);

    //Serial.print(data);
    //Serial.print(str); 


    // パケット送信
    //udp.beginPacket(udpReturnAddr, udpReturnPort);
    udp.beginPacket(remoteIP, REMOTE_PORT);
    udp.write(data);
    udp.endPacket();
    //Serial.print(data);
    digitalWrite(5, LOW);   //アクセスランプ消灯　シルクはLED
    
    str = str0; //文字列変数の中身を初期化

  }
  
}
