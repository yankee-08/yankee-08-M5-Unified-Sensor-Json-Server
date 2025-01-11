# M5-Unified-Sensor-Json-Server

M5Stackをセンサデータを返すJSONサーバ化しています。  
`PlatformIO`＋`M5Unified`の組み合わせで作っています。  
以下のQiita記事に、手順をまとめています。  

[DBを介さずにセンサデータをGrafanaで直接可視化する](https://qiita.com/yankee/items/995f559fbd7b4cb0e749)

<img src="https://qiita-user-contents.imgix.net/https%3A%2F%2Fqiita-image-store.s3.ap-northeast-1.amazonaws.com%2F0%2F371217%2Ffac9df7b-6022-9c6a-9497-527c29137a40.png?ixlib=rb-4.0.0&auto=format&gif-q=60&q=75&s=03ee86c2dae3147cc47df1d2384005b9" width="600" />

## Description

- M5Stackに温湿度センサユニット（ENV II）をつないで、温湿度データを取得
- 取得したデータを過去10件分、リングバッファに保存
- M5StackをWebサーバ化し、HTTPリクエストを受け付けた場合にセンサデータをJSON形式で返す

## Requirement
### Hardware

M5Stackシリーズ ＋ ENV Unit
`platform.ini`ファイルを追記することで、色々なM5Stackシリーズで動作すると思われますが、一部センサに依存するライブラリを使用しているため、追加のライブラリを入れる必要がある可能性があります。

動作確認済み機種

- M5Stack ATOMS3＋ENV II Unit

### Software

- Visual Studio Code
- PlatformIO

## Usage

- [main.h](https://github.com/yankee-08/yankee-08-M5-Unified-Sensor-Json-Server/blob/main/include/main.h) 内のSSIDやIPアドレスを各自の環境に合わせて設定してください

## License

[MIT](https://github.com/yankee-08/M5-Unified-SwitchBot-Meter-BLE/blob/main/LICENSE)  

## Author

[yankee](https://github.com/yankee-08)  
