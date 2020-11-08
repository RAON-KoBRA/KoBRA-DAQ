# KoBRA DAQ
   Based on the MIDAS library,
 
 ## MIDAS 개요
 
 
 
 ## KoBRA DAQ F3 Chamber test varient
 
![kobra_main](https://user-images.githubusercontent.com/38948046/94102199-8e90d180-fe6c-11ea-8d35-39388c51dfa4.png)
  <figcaption>  그림1. KoBRA DAQ Status Page</figcaption>
  
  
  
  
 


### Operation environment

    Ubuntu 18.04.4 LTS
    AMD Ryzen 7 3700X 8-Core Processor/ AMD Ryzen 9 3900x 12-Core Processor
    5.3.0-51-generic 
    gcc version 7.5.0 (Ubuntu 7.5.0-3ubuntu1~18.04)
    cmake version 3.17.1



 
  ## 업데이트(2020-09-21)
 이용자 검출기 대응 부분 추가( Silicon array) : mfe_silicon_ary추가,  DAQ에서 equipment를 추가하여 따로 분리시킴. Silicon arrary의 경우 Back scattering되는 Heavy ion의 E & dE를 측정하여 (d,p) reaction의 differential cross section을 구하는데 이용됨. 
 
  odb writing 업데이트, equipment 별 다른 데이터를 유지시키는 등 FPGA 자원을 따로 관리하기 위해 ODB/CUSTOM 영역을 추가하여 FPGA 입력값을 DAQ live buffer에 저장함. 해당 내용은 Scaler에 관련된 부분이기 때문에 mfe_v2495에 추가됨. 차후 업데이트 가능성, 특히 본문 작성자가 없을때 DAQ 시스템의 지속적인 유지 관리를 위하여 MIDAS main structure는 변경하지 않고, user modification level에서 해결하는 방향으로 진행. 차후 Scaler statistics의 경우는 자체적으로 정의한 custom ODB에서 관리함. 하지만, counting variable의 경우는 Equipment에서 자원 관리가 이루어 지기 때문에, 혼동이 발생할 수 있어, 실제 장치 운용이 이루어지면서 자원관리 방안에 대해 다시 고민할 필요가 있어보임. 추가적으로, 제어시스템에 접근하여 필요한 Variable에 접근하는 방인이 이루어질 것임.
 
 Custom page 업데이트, equipment 별로 다른 livetime을 표시해주며, beamrate, timetag, down scaled rate를 따로 표시함. FIFO buffer 초기화 버튼 추가됨. 
  현재 Trigger counting과 Midas defualt counting이 mismatch 되고있어 진단이 필요함. 제어시스템에 접근하여 variable들을 습득하여 시각적 보조 효과를 높일 예정.
 

 ADC를 제외한 전체 테스트 결과 전체적인 작동상태 양호. KoBRA DAQ 1.0 단계 완성(?) 이후 추가 기능들을 점진적으로 확대해 나갈것. 



## References
- [MIDAS wiki]( https://midas.triumf.ca/MidasWiki/index.php/Main_Page)


