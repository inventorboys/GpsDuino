# GpsDuino
Arduino ile gps yapımı

Gereken Malzemeler
neo6mv gps
arduino uno
gps için seramik anten
(linkler link repositorysinde)

gpsle ilk konum bulma 2-3 dk (cold start) 
ilk kullanışta bazen 10-15dk beklemeniz gerebilir (first start)
konum buluduktan sonra 10-15 dk içinde tekrar konum bulmanız 15-20 saniye sürücektir (hot start)
onun dışında normal koşullarda 25-45 saniye içinde konum bulunur (mild start)

arduinoya kodu yükledikten sonra modülün rx tx vcc ve gnd pinlerini bağlayın
rx 9 tx 8 gnd gnd vcc 5v şeklinde bağlayın
ÖNEMLİ EĞER KARTINIZDA VCC YERİNE 3.3V YAZIYORSA ARDİNODAKİ 3.3V A TAKMANIZ GEREKİYOR 3.3V YAZANI 5V A BAĞLAMANIZ KARTINIZI YAKICAKTIR

Ardından yapmanız gerekn şey ino dosyasını açmak kod yğklemek ve beklemek 
