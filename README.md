# KoBRA DAQ
   KoBRS DAQ는 MIDAS를 기반으로 
 
 ## MIDAS 개요
 
 ## 업데이트(2020-05-08)
 
   기존 개발된 DAQ는 구 버전의 Midas package를 이용하였고, 이로 인하여 c와 c++가 섞인 복잡한 구조를 갖고 있다. 근래에 이르러 컴파일러, 라이브러리, 드라이버, os 등이 업데이트 되면서 기존의 DAQ는 다음과 같은 문제점을 발생시킨다.

1. 정상적인 컴파일 불가: 주요 함수 syntax 변경, 헤더파일 탐색 불가, 변수명 중복 선언, 변수 타입 오류
2. Freezing: 데이터 습득 도중 특정 조건이 발생하면 즉시 freeze되어 측정 불가 ( e.g. ADC 혹은 TDC 모듈당 입력 채널이 9개가 넘어가면 발생) 
3.

현재,


### Operation environment

    Linux raspberrypi 4.4.34-v7+ armv7l GNU/Linux
    gcc(Raspbian 4.9.2-10) 4.9.2



## References
- [MIDAS wiki]( https://midas.triumf.ca/MidasWiki/index.php/Main_Page)


