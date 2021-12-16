## 何を作ったか              
              
講義内で使用したmyledを改造して複数のLEDを扱える様にしました．    
又，ブレッドボードとRaspberryPiの周辺を覆う様に円錐台状に丸めたA4の紙を設置することで，    
簡易照明の様なインテリアとして活用出来るようにしました．             
              
## 使用環境              
              
* RaspberryPi3B+              
* Ubuntu Server 20.04LTS              
              
## デバイスドライバのインストール方法              
              
* 当リポジトリをcloneして下さい．              
```        
git clone https://github.com/keito1029/myled.git        
```             
    
* install.shを実行する事で古いデバイスドライバの設定を削除し，  一般ユーザが扱える様に設定を更新，反映する事ができます．           
```             
sudo sh install.sh              
```             
              
## 操作方法              
    
* red,blue,green,whiteの内，点灯させたいLED色の頭文字を以下の様に入力して実行します．      
            
```             
echo r > /dev/myled0            
```             
              
* purple,yello,ligth blueも操作する事が可能ですが緑の光源が弱い為，上記の結果とほぼ同じ様に点灯します．            
    
## ハード作成方法             
            
1． 赤:330Ωの抵抗をGPIO25とGNDに接続            
2． 青:660Ωの抵抗をGPIO24とGNDに接続            
3． 緑:10Ωの抵抗をGPIO23とGNDに接続            
4． 紙を円錐台になるように丸め，テープか何かで接着          
5． 4で作成したもので回路とRaspberryPiを覆う様に設置    
          
           
## Note          
          
テスターのダイオードチェックを活用する際に，回路がGPIOに接続した状態でデバイスドライバにLEDを消灯させる設定をしていると，LEDは点灯しません．          
これはrmmodコマンドを使って直接削除しても継続しますが，      
デバイスドライバをインストールし直せば設定を再び反映させる事ができます．    
つまり，ダイオードチェックでLEDが点灯しなければデバイスドライバの設定が正常に機能しているという事になります．    
          
        
        
      
    
  

