# KoBRA DAQ
   Based on the MIDAS library, KoBR
 
 ## MIDAS 개요
 
 ## 업데이트(2020-05-08)
 
   기존 개발된 DAQ는 구 버전의 Midas package를 이용하였고, 이로 인하여 c와 c++가 섞인 복잡한 구조를 갖고 있다. 근래에 이르러 컴파일러, 라이브러리, 드라이버, os 등이 업데이트 되면서 기존의 DAQ는 다음과 같은 문제점을 발생시킨다.

1. 컴파일 오류발생: 주요 함수 syntax 변경, 헤더파일 탐색 불가, 변수명 중복 선언, 자료형 오류
2. Freezing: 데이터 습득 도중 특정 조건이 발생하면 V2718모듈의 Rx/Tx 가 중단되면서 측정 불가 ( e.g. ADC 혹은 TDC 모듈당 입력 채널이 9개가 넘어가면 발생) 


현재 fronend와 mfe가 c++기반으로 작동하도록 변경되었고, 이에 맞추어 자료형과 변수명도 수정되었다. 펄스 생성기를 이용한 시험 결과, 기존의 문제가 해결되었다.

 p.s. 최신 Midas와 Ubuntu apt 상에 있는 Mongoose web server 버전 차이로 인해 암호화 문제가 발생한다. 이로 인해, 구 mhttpd 를 사용중이다.  


### Operation environment

    Linux raspberrypi 4.4.34-v7+ armv7l GNU/Linux
    gcc(Raspbian 4.9.2-10) 4.9.2



## References
- [MIDAS wiki]( https://midas.triumf.ca/MidasWiki/index.php/Main_Page)


