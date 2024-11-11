import 'dart:ffi';

import 'package:flutter_application_1/model/app/content/item.dart';
import 'package:flutter_application_1/model/app/setting/timer_setting.dart';

class Roof extends Item {
  String sensorTitle = "Cảm biến mưa";
  TimerSetting timerSetting;

  Roof(
      {required super.iconPath,
      required super.title,
      required super.isOnDevice,  
      required super.isAuto,
      required super.haveATimer,
      required this.timerSetting,
      });
}

