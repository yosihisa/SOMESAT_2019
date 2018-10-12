# SOMESAT 種コン2019

## これはなに
SOMESAT種コン2019関係のプロジェクトを管理するリポジトリです。  
各CanSatの基板データとかプログラムとかいろいろ  
<br>
<font color="Red">注意: ESFF(仮称)はこのリポジトリではありません。</font>

## 編集するときは？  
自分のブランチを作ってそこにpushしてください。  
その後masterにプルリク送ってください。  
**masterを直接書き換えないでください。**   
## 何があるの？
ディレクトリ構成は以下の通り  

- PCB  
基板データ関係
    - EDAMAME_S  
    えだまめSの基板データ  
    n_yosihisa

    - EDAMAME_WiFimodule  
    ZUNDA～えだまめ!!に使われていたESP8266無線モジュールの基板データ  
    yuuitirou528  

    -  	EDAMAMEv2  
    画像認識機の基板データ  
    n_yosihisa  

- firmware  
プログラム関係  
    - EDAMAME_S  
    環境  
    ・ Arduino IDE  
    ・ ESP32  
    えだまめSのスケッチ  
    n_yosihisa

    - EDAMAME_WiFimodule  
    環境  
    ・ Arduino IDE  
    ・ ESP8266  
    ZUNDA～えだまめ!!に使われていたESP8266無線モジュールのスケッチ  
    yuuitirou528  

    -  EDAMAMEv2  
    環境  
    ・C(HAL)  
    ・Atollic TrueSTUDIO  
    ・CubeMX  
    ・STM32F446   
    画像認識機のプログラム  
    n_yosihisa  

- documents  
文書関係  
仕様書など  

### ファイルを書き替えるときの注意  
基本的に自分の担当しているもののディレクトの中身以外は変更しないように (2018/10/5)
***

## 著作権などについて
基本的に著作権は各作者に帰属します。  
種コンが終わるまで自分が書いたもの以外原則公開禁止です。  
ただし、種コン終了後、このリポジトリをpublicにする可能性があります。  