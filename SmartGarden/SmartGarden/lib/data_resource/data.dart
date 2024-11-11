import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:flutter_application_1/model/app/content/item.dart';
import 'package:flutter_application_1/model/app/content/light.dart';
import 'package:flutter_application_1/model/app/content/mist_spray.dart';
import 'package:flutter_application_1/model/app/content/roof.dart';
import 'package:flutter_application_1/model/app/content/watering.dart';
import 'package:flutter_application_1/model/app/setting/level_setting.dart';
import 'package:flutter_application_1/model/app/setting/range_setting.dart';
import 'package:flutter_application_1/model/app/setting/timer_setting.dart';

class Data {
  late List<Item> items;

  Future<List<Item>> getData() async {
    DatabaseReference ref = FirebaseDatabase.instance.ref();
    final snapshot = await ref.get();
    if (snapshot.exists) {
      final value = snapshot.value;
      final devices = (value as dynamic)['device'];
      final sensor = (value as dynamic)['sensor'];
      final bom = devices['bom_tuoi_cay'];
      final den = devices['den_quang_hop'];
      final mai_che = devices['mai_che'];
      final phun_suong = devices['phun_suong'];

      items = [
        Watering(
            iconPath: 'images/watering-can128green.png',
            sensorTitle: "Độ ẩm đất",
            sensorValue: sensor['do_am_dat'] as int,
            title: 'Tưới cây',
            isOnDevice:
                bom['nguon'] as bool, // có bật thiết bị không (thủ công)
            isAuto: bom['auto'] as bool, // có đang ở chế độ auto không
            haveATimer: bom['hen_gio'] as bool, // có hẹn giờ không
            rangeSetting: RangeSetting(
                rangeValue: bom['nguong_do_am_dat'] as int,
                settingTitle: "Thiết lập ngưỡng độ ẩm đất"),
            timerSetting: TimerSetting(
                timerBegin: TimeOfDay(
                    hour: bom['thoi_gian']['bat_bom']['hour'] as int,
                    minute: bom['thoi_gian']['bat_bom']['min'] as int),
                timerEnd: TimeOfDay(
                    hour: bom['thoi_gian']['tat_bom']['hour'] as int,
                    minute: bom['thoi_gian']['tat_bom']['min'] as int),
                settingTitle: "Hẹn giờ tưới cây"),
            speedSetting: LevelSetting(
                settingTitle: "Tốc độ bơm",
                lowLevelLabel: "1",
                mediumLevelLabel: "2",
                highLevelLabel: "3",
                value: bom['toc_do_bom'] as int)),
        Light(
            iconPath: 'images/brightness128yellow.png',
            title: 'Đèn',
            isOnDevice: den['nguon'] as bool,
            isAuto: den['auto'] as bool,
            haveATimer: den['hen_gio'] as bool,
            timerSetting: TimerSetting(
                settingTitle: "Hẹn giờ bật/tắt đèn",
                timerBegin: TimeOfDay(
                    hour: den['thoi_gian']['bat_den']['hour'] as int,
                    minute: den['thoi_gian']['bat_den']['min'] as int),
                timerEnd: TimeOfDay(
                    hour: den['thoi_gian']['tat_den']['hour'] as int,
                    minute: den['thoi_gian']['tat_den']['min'] as int),
                ),
            adaptSetting: LevelSetting(
                settingTitle: "Thiết lập độ nhạy",
                lowLevelLabel: "Gần tối",
                mediumLevelLabel: "Nhá nhem",
                highLevelLabel: "Tối",
                value: den['do_nhay'] as int)),
        Roof(
            iconPath: 'images/roof128red.png',
            title: 'Mái che',
            isOnDevice: mai_che['nguon'] as bool,
            isAuto: mai_che['auto'] as bool,
            haveATimer: mai_che['hen_gio'] as bool,
            timerSetting: TimerSetting(
                settingTitle: "Hẹn giờ đóng/mở mái che",
                timerBegin: TimeOfDay(
                    hour: mai_che['thoi_gian']['dong_mai_che']['hour'] as int,
                    minute: mai_che['thoi_gian']['dong_mai_che']['min'] as int),
                timerEnd: TimeOfDay(
                    hour: mai_che['thoi_gian']['mo_mai_che']['hour'] as int,
                    minute: mai_che['thoi_gian']['mo_mai_che']['min'] as int),
                )),
        MistSpray(
            iconPath: 'images/pesticide.png',
            title: 'Phun sương',
            isOnDevice: phun_suong['nguon'] as bool,
            isAuto: phun_suong['auto'] as bool,
            haveATimer: phun_suong['hen_gio'] as bool,
            humidValue: sensor['do_am_moi_truong'] as int,
            speedSetting: LevelSetting(
                settingTitle: "Tốc độ phun",
                lowLevelLabel: "1",
                mediumLevelLabel: "2",
                highLevelLabel: "3",
                value: phun_suong['toc_do_phun'] as int
                ),
            timerSetting: TimerSetting(
                settingTitle: "Hẹn giờ bật/tắt phun",
                timerBegin: TimeOfDay(
                    hour: phun_suong['thoi_gian']['bat_phun']['hour'] as int,
                    minute: phun_suong['thoi_gian']['bat_phun']['min'] as int),
                timerEnd: TimeOfDay(
                    hour: phun_suong['thoi_gian']['tat_phun']['hour'] as int,
                    minute: phun_suong['thoi_gian']['tat_phun']['min'] as int),
                
                ),
            rangeSettings: 
              RangeSetting(
                  rangeValue: phun_suong['nguong_do_am_mt'] as int, settingTitle: "Thiết lập ngưỡng độ ẩm không khí")
            )
      ];

      return items;
    } else {
      return [];
    }
  }
}
