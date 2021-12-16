## 何を作ったか          
          
講義内で使用したmyledを改造して複数のLEDを扱える様にし，それにA4の紙を巻き付けることで簡易照明を作成した          
          
## 使用環境          
          
* RaspberryPi3B+          
* Ubuntu Server 20.04LTS          
          
## 使用方法          
          
* 当リポジトリをclone          
```    
git clone https://github.com/keito1029/myled.git    
```         
* シェルの実行          
```         
sudo sh install.sh          
```         
          
* red,blue,green,whiteの内，光らせたいLED色の頭文字をechoする          
  purple,yello,ligth blueも作成したが緑の光源が弱く，上記とほぼ同じ結果となる        
        
```         
echo r > /dev/myled0        
```         
          
## ハード作成方法         
        
1． 赤:330Ωの抵抗をGPIO25とGNDに接続        
2． 青:660Ωの抵抗をGPIO24とGNDに接続        
3． 緑:10Ωの抵抗をGPIO23とGNDに接続        
4． 紙を円錐台になるように丸め，テープか何かで接着      
      
       
## Note      
      
回路とGPIOに接続した状態でデバイスドライバを起動していると，ダイオードチェックしてもLEDが光らない      
逆に光れば，デバイスドライバが機能していない      
      
    
    
  

