# えだまめS ミッション機 OBCボード  
## 要求  
・フライトピンの状態をIMU/COMボードに伝えられる  
・GPSのGPGGAセンテンスを10HzでSDカードに記録できる  
・IMU/COMボードから送られてきたコマンドを実行できる  

## 仕様
### 動作シーケンス  
1. 電源投入  
2. GPSモジュールの高速化/Missionボードの動作をOFFに  
3. loopの実行  
    - (有効なら)GPSを受信  
    - (有効なら)GPSを記録  
    - フライトピンの情報の更新  
- serialの割込み
    - コマンドの受信  
    - コマンドの実行(Missionボードへのコマンド・GPS記録の有/無効化)  


### GPSの記録    
- GPS記録が有効なとき  
 10HzでGPGGAの行をそのままSDカードに記録する。  
 ファイル名は”番号.csv"。  
 電源投入でファイル更新 それ以外は同じファイルに書き続ける。  

### IMU/COMからOBCへのコマンド  
UARTを用いる。コマンド数が少ないので全て1バイトで完結させる。  
- '4' ネギ振り OFF  
- '5' ネギ振り ON 通常  
- '6' ネギ振り ON ゲーミング仕様  
- '9' GPS記録 有効化  
- '0' GPS記録 無効化  

 
### OCBからIMU/COMへのコマンド
送信する情報はフライトピンのみ。UARTを用いる。
- 'H' 動作中  
- 'L' 待機中  


### OBCからミッションへのコマンド  
状態は3通り。GPIOを用いる。  
| 16 SCL | 17 SDA | Mode |  
|:--:|:--:|:---:|  
| L | - | OFF |  
| H | L | ON(通常) |  
| H | H | ゲーミング |  


# という風にしたかったが実装が間に合わなかったので  
OBC→IMU IMU→OBCともにGPIOで通信  
内容は
- フライトピンの状態
- LEDの動作コマンド(通常orゲーミング仕様)  
機体へのアップリンクコマンドは  
- 記録開始  
- 記録停止  
- 高度オフセット  
- LED 通常  
- LED ゲーミング仕様  
とする。  
GPS関連のコマンドは省略。常時記録とする。