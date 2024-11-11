import 'package:flutter_application_1/model/app/content/item.dart';
import 'package:flutter_application_1/model/app/setting/level_setting.dart';
import 'package:flutter_application_1/model/app/setting/timer_setting.dart';

class Light extends Item {

  String sensorTitle = "Cảm biến ánh sáng";
  TimerSetting timerSetting; 
  LevelSetting adaptSetting;
  

  Light(
      {required super.iconPath,
      required super.title,
      required super.isOnDevice,
      required super.isAuto,
      required super.haveATimer,
      required this.timerSetting,
      required this.adaptSetting,
    });
}


