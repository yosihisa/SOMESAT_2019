## SOMESAT 種コン2019

# CanSat標準バスと超小型CanSat規格の策定について

## 1.目的  
1.1 超小型CanSat規格を策定する目的  
* 複数の小型ローバーを同時に放出するCanSatの大きさやキャリアの規格を定めることにより、この規格に準拠するCanSatであればどのキャリアにも格納することができるようになる。これによって複数のチームや団体で共同で投下実験を行う際の連携が簡単になる。  

1.2 CanSat標準バスの目的  
* CanSat標準バスを開発することにより、初めてCanSatを扱う人でも短期間で効率的に学習ができるようにする。  
* やりたいミッションがある場合に標準バスを使うことによりバスの開発期間を短縮しミッションに専念することができるようになる。  

## 2.要求
2.1  超小型CanSatの規格要求案  
1. 1機あたりの大きさはARLISS規格で4機　種コン規格で6機　加太規格で8機を格納できるような円筒形とする。  
2. ARLISS,種コン,加太のそれぞれの大きさに合わせたキャリアの形状を詳細に定める。  
3. ローバー形の場合、従来の3Dプリンタで造形したタイヤ,メジャーを持ちいた尻尾などの形状を基本形とする。  

2.2 CanSat標準バスの要求案  
1. 前述した超小型CanSat規格に完全に対応していること。  
2. Arduinoで開発できることが望ましい。  
3.  一般的なランバックを標準構成のままできるような構成とする。具体的にはGPS,コンパス,ロガー,モータードライバなどの機能を内包する。  
4.  拡張性があり他のCanSatのコアボードとして流用しやすくなっていること。  

## 3.CanSat標準バス(仮称:えだまめS)の現時点での具体的な案について
* えだまめSのボードはコアボードと拡張ボードの2枚からなる。  
* コアボードには メインMPU, GPS, コンパス, ロガー(microSD), 拡張用GPIO を搭載  
* 拡張ボードには 電池, 電源回路, モータードライバ, 展開機構 を搭載  

## 4.想定する具体的な利用方法  
    超小型CanSat規格 → SOMESATが中心となって策定する"規格"
    CanSat標準バス → SOMESATが製造する"製品"  
- 新人教育用ローバー形CanSatの場合  
    - 超小型CanSat規格準拠  
    - えだまめS コア+拡張ボードを使用  
- 追加機能のある小型CanSatの場合
    - 超小型CanSat規格準拠  
    - えだまめS コアボード のみ使用  
- ミッションベースの小型CanSatの場合
    - 超小型CanSat規格準拠  
    - えだまめS コアボードを使ってもよい  
- 大型CanSatの場合
    - 超小型CanSat規格非準拠
    - えだまめS コアボードを使ってもよい

## 5.メインミッション  
### 「超小型CanSat規格とCanSat標準バスの実証」
    従来のCanSatにおける複数機の目的は信頼性の向上であったが、今回は従来の考え方を変え  低コストで複数のミッションを同時に実験することができるようになる というのを実証・提案する。  
    超小型CanSat規格を使って開発することの有用性を証明する。  
    可能であればえだまめSを種コン内で販売する。  

### 機体構成案
ARLISS規格の4機のキャリアを使用  

搭載する機体は以下の4種類（全機超小型CanSat規格準拠）  
1. えだまめSコア+拡張ボードを使用したローバー機  
　これを最低限の機能を搭載したローバー型超小型CanSatのリファレンスモデルとする。  
2. えだまめSコア+水ロケでの使用を想定したIMU  
　これを超小型CanSat規格対応でえだまめSを使用したミッション機の具体例とする。  
3. えだまめ!!の画像認識機を超小型CanSat規格に対応させたもの  
　超小型CanSat規格に準拠しつつもえだまめSを用いない高性能な機体も使えるという具体例とする。  
4. LoRa変調通信モジュール+Lチカネギ振りはちゅねミク+カメラを搭載したミッション機（えだまめSコアボードを使うかは不明）　  
　LoRa衛星のBBMとする。  