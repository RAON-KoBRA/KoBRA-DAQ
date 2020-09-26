# KoBRA DAQ
   Based on the MIDAS library,
 
 ## MIDAS 개요
 
 
 
 ## KoBRA DAQ Ver. 1.0
 
![kobra_main](https://user-images.githubusercontent.com/38948046/94102199-8e90d180-fe6c-11ea-8d35-39388c51dfa4.png)
  <figcaption>  그림1. KoBRA DAQ Status Page</figcaption>
  
  
  
  
 
 ## 업데이트(2020-05-08)
 
   기존 개발된 DAQ는 구 버전의 Midas package를 이용하였고, 이로 인하여 c와 c++가 섞인 복잡한 구조를 갖고 있다. 근래에 컴파일러, 라이브러리, 드라이버, os 등이 업데이트 되면서 기존의 DAQ는 다음과 같은 문제점을 발생시킨다.

1. 컴파일 오류발생: 주요 함수 syntax 변경, 헤더파일 탐색 불가, 변수명 중복 선언, 자료형 오류
2. Freezing: 데이터 습득 도중 특정 조건이 발생하면 V2718모듈의 Rx/Tx 가 중단되면서 측정 불가 ( e.g. ADC 혹은 TDC 모듈당 입력 채널이 9개가 넘어가면 발생) 


현재 fronend와 mfe가 c++기반으로 작동하도록 변경되었고, 이에 맞추어 자료형과 변수명도 수정되었다. 펄스 생성기를 이용한 시험 결과, 기존의 문제가 해결되었다.

 p.s. 최신 Midas와 Ubuntu apt 상에 있는 Mongoose web server 버전 차이로 인해 암호화 문제가 발생한다. 이로 인해, 구 mhttpd 를 사용중이다.  


### Operation environment

    Ubuntu 18.04.4 LTS
    AMD Ryzen 7 3700X 8-Core Processor/ AMD Ryzen 9 3900x 12-Core Processor
    5.3.0-51-generic 
    gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)
    cmake version 3.17.1


 ## 업데이트(2020-06-30)
 
V2718 VME bridge의 Daisy chian 연결(optical link)을 위해 Identification 부분을 수정함; MIDAS internal function에서 device group 구분자인 (Bhandle) HdB 값을 optical link 1과 2로 구분지어 정의됨.  현재 PPAC edge correction 부분은 소거된 상태로, 차후 PID 부분과 한번에 업데이트  


## 업데이트(2020-07-05).
 Shell inotofiy를 이용, counting PC와 control PC의 하드에 Raw data (Run*.mid.iz4 file)을 저장하도록 start_daq 구문에 추가함(RAID 1 방식) 현재 inotify 방식으로 인해 개선점은, kill_daq가 이루어질때, 특정 시간만큼 sleep 되게 설정하였기 때문에 효율이 떨어짐, 차후 SCP가 종료되면 즉각적으로 탈출하도록 개선이 필요.
 
 
 ## 업데이트(2020-07-20)
 V2495 FPGA 추가에 따른 mfe_v2495 원형 추가. I/O 테스트 진행. V2714 및 DAQ에서 V2495와의 통신은 양호한 것으로 확인됨. 하지만, Register read function 사용시, 컴퓨터  latency dependance가 존재하여 정확한 Count값을 받을 수 없음. 이에 대한 대안으로, Scaler function에 의존하여 Parallel 하게 Count 가 이루어지는 상태에서, V2718의 Block transfer method를 통해서 FPGA IO buffer에 직접 접근하는 방법을 적용할 필요가 있음.
 
  ## 업데이트(2020-08-03)
mfe_v2495 업데이트. 전 업데이트에서 언급한 방법대로, V2714 Block transfer 알고리즘의 File I/O를 이용한 FPGA FIFO 메모리 직접 접근에 성공함. Counting value test결과 작동상태 양호. 100 MHz External source를 이용해서 초당 100 M counting 이 이루어지는 것을 확인함, resoulution은 few count 수준으로 양호 (10 ns 이내)

 ## 업데이트(2020-08-31)
 mfe_v2495 업데이트: Middleware의 Internal function인 Scaler_FIFO function을 이용하여 FPGA FIFO 메모리에 접근하는 방법으로 변경됨에 따라 input 채널 구분이 더 용이해짐.
 
 DAQ mhttpd에서 Custom page추가 (Custom 폴더). 현재 odbedit configuration을 통해 경로가 bind 되어 있으나, 차후 start_daq에서 형성하도록 코드 추가 필요.
 
  ## 대규모 업데이트(2020-09-21)
 이용자 검출기 대응 부분 추가( Silicon array) : mfe_silicon_ary추가,  DAQ에서 equipment를 추가하여 따로 분리시킴. Silicon arrary의 경우 Back scattering되는 Heavy ion의 E & dE를 측정하여 (d,p) reaction의 differential cross section을 구하는데 이용됨. 
 
  odb writing 업데이트, equipment 별 다른 데이터를 유지시키는 등 FPGA 자원을 따로 관리하기 위해 ODB/CUSTOM 영역을 추가하여 FPGA 입력값을 DAQ live buffer에 저장함. 해당 내용은 Scaler에 관련된 부분이기 때문에 mfe_v2495에 추가됨. 차후 업데이트 가능성, 특히 본문 작성자가 없을때 DAQ 시스템의 지속적인 유지 관리를 위하여 MIDAS main structure는 변경하지 않고, user modification level에서 해결하는 방향으로 진행. 차후 Scaler statistics의 경우는 자체적으로 정의한 custom ODB에서 관리함. 하지만, counting variable의 경우는 Equipment에서 자원 관리가 이루어 지기 때문에, 혼동이 발생할 수 있어, 실제 장치 운용이 이루어지면서 자원관리 방안에 대해 다시 고민할 필요가 있어보임. 추가적으로, 제어시스템에 접근하여 필요한 Variable에 접근하는 방인이 이루어질 것임.
 
 Custom page 업데이트, equipment 별로 다른 livetime을 표시해주며, beamrate, timetag, down scaled rate를 따로 표시함. FIFO buffer 초기화 버튼 추가됨. 
  현재 Trigger counting과 Midas defualt counting이 mismatch 되고있어 진단이 필요함. 제어시스템에 접근하여 variable들을 습득하여 시각적 보조 효과를 높일 예정.
 

 ADC를 제외한 전체 테스트 결과 전체적인 작동상태 양호. KoBRA DAQ 1.0 단계 완성(?) 이후 추가 기능들을 점진적으로 확대해 나갈것. 



## References
- [MIDAS wiki]( https://midas.triumf.ca/MidasWiki/index.php/Main_Page)


