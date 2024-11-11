import 'package:flutter_application_1/model/app/content/item.dart';
import 'package:flutter_application_1/model/app/setting/level_setting.dart';
import 'package:flutter_application_1/model/app/setting/range_setting.dart';
import 'package:flutter_application_1/model/app/setting/timer_setting.dart';

class MistSpray extends Item {
  String tempTitle = "Nhiệt độ môi trường";
  String humidTitle = "Độ ẩm không khí";
  int humidValue;
  RangeSetting rangeSettings; 
  TimerSetting timerSetting;
  LevelSetting speedSetting;

  MistSpray(
      {required super.iconPath,
      required super.title,
      required super.isOnDevice,
      required super.isAuto,
      required super.haveATimer,
      required this.humidValue,
      required this.timerSetting,
      required this.rangeSettings,
      required this.speedSetting,
      }); // phun sương
}
