import 'package:flutter_application_1/model/app/content/item.dart';
import 'package:flutter_application_1/model/app/setting/level_setting.dart';
import 'package:flutter_application_1/model/app/setting/range_setting.dart';
import 'package:flutter_application_1/model/app/setting/timer_setting.dart';



class Watering extends Item {
  String sensorTitle;
  int sensorValue;
  TimerSetting timerSetting;
  RangeSetting rangeSetting;
  LevelSetting speedSetting;

  Watering(
      {required super.iconPath,
      required super.title,
      required super.isOnDevice,
      required super.isAuto,
      required this.sensorTitle,
      required this.sensorValue,
      required super.haveATimer,
      required this.timerSetting,
      required this.speedSetting,
      required this.rangeSetting});
}
