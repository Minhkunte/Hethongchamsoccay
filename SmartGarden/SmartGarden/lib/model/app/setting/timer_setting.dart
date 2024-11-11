import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

class TimerSetting {
  String settingTitle;
  TimeOfDay timerBegin;
  TimeOfDay timerEnd;

  TimerSetting(
      {required this.settingTitle,
        required this.timerBegin,
        required this.timerEnd
      });
}
