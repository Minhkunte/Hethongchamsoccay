import 'dart:ffi';

import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter_application_1/model/app/setting/timer_setting.dart';

class TimerSettingContainer extends StatefulWidget {
  TimerSetting timerSetting;
  TimerSettingContainer({super.key, required this.timerSetting});

  @override
  _TimerSettingContainerState createState() => _TimerSettingContainerState();
}

class _TimerSettingContainerState extends State<TimerSettingContainer> {
  late DatabaseReference _deviceRef;
  bool henGio = false;
  late Function toggleTimer; // hàm bật tắt chức năng hẹn giờ

  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    init();
  }

  Future<void> init() async {
    _deviceRef = FirebaseDatabase.instance.ref("device");
    final snapshot = await _deviceRef.get();
    if (snapshot.exists) {
      if (widget.timerSetting.settingTitle == "Hẹn giờ tưới cây") {
        henGio = (snapshot.value as dynamic)['bom_tuoi_cay']['hen_gio'];
        toggleTimer = (bool value) {
          DatabaseReference ref =
              FirebaseDatabase.instance.ref("device/bom_tuoi_cay/hen_gio");
          ref.set(value);
          setState(() {
            henGio = value;
          });
        };
      }

      if (widget.timerSetting.settingTitle == "Hẹn giờ đóng/mở mái che") {
        henGio = (snapshot.value as dynamic)['mai_che']['hen_gio'];
        toggleTimer = (bool value) {
          DatabaseReference ref =
              FirebaseDatabase.instance.ref("device/mai_che/hen_gio");
          ref.set(value);
          setState(() {
            henGio = value;
          });
        };
      }

      if (widget.timerSetting.settingTitle == "Hẹn giờ bật/tắt đèn") {
        henGio = (snapshot.value as dynamic)['den_quang_hop']['hen_gio'];
        toggleTimer = (bool value) {
          DatabaseReference ref =
              FirebaseDatabase.instance.ref("device/den_quang_hop/hen_gio");
          ref.set(value);
          setState(() {
            henGio = value;
          });
        };
      }

      if (widget.timerSetting.settingTitle == "Hẹn giờ bật/tắt phun") {
        henGio = (snapshot.value as dynamic)['phun_suong']['hen_gio'];
        toggleTimer = (bool value) {
          DatabaseReference ref =
              FirebaseDatabase.instance.ref("device/phun_suong/hen_gio");
          ref.set(value);
          setState(() {
            henGio = value;
          });
        };
      }
    }
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Text(widget.timerSetting.settingTitle),
            SizedBox(
              width: 6,
            ),
            Switch(
                value: henGio,
                onChanged: (bool value) {
                  toggleTimer(value);
                })
          ],
        ),
        SizedBox(
          height: 60,
        ),

        !henGio
            ? Icon(
                CupertinoIcons.clock,
                size: 46,
              )
            : Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  Column(
                    children: [
                      Text("Hẹn giờ bật"),
                      OutlinedButton(
                          onPressed: () async {
                            final TimeOfDay? timeOfDay = await showTimePicker(
                                context: context,
                                initialTime: widget.timerSetting.timerBegin,
                                initialEntryMode: TimePickerEntryMode.dialOnly);

                            if (timeOfDay != null) {
                              setState(() {
                                widget.timerSetting.timerBegin = timeOfDay;
                              });
                            }
                          },
                          child: Text(
                              "${widget.timerSetting.timerBegin.hour}:${widget.timerSetting.timerBegin.minute}"))
                    ],
                  ),
                  Column(
                    children: [
                      Text("Hẹn giờ tắt"),
                      OutlinedButton(
                          onPressed: () async {
                            final TimeOfDay? timeOfDay = await showTimePicker(
                                context: context,
                                initialTime: widget.timerSetting.timerEnd,
                                initialEntryMode: TimePickerEntryMode.dialOnly);
                            if (timeOfDay != null) {
                              setState(() {
                                widget.timerSetting.timerEnd = timeOfDay;
                              });
                            }
                          },
                          child: Text(
                              "${widget.timerSetting.timerEnd.hour}:${widget.timerSetting.timerEnd.minute}"))
                    ],
                  )
                ],
              ), // Hiển thị giá trị tối đa
        SizedBox(
          height: 20,
        ),
        
        henGio
            ? FilledButton(onPressed: () {
              if (widget.timerSetting.settingTitle == "Hẹn giờ tưới cây") {

                DatabaseReference refHourOn = FirebaseDatabase.instance.ref("device/bom_tuoi_cay/thoi_gian/bat_bom/hour");
                refHourOn.set(widget.timerSetting.timerBegin.hour);

                DatabaseReference refMinOn = FirebaseDatabase.instance.ref("device/bom_tuoi_cay/thoi_gian/bat_bom/min");
                refMinOn.set(widget.timerSetting.timerBegin.minute);

                DatabaseReference refHourOff = FirebaseDatabase.instance.ref("device/bom_tuoi_cay/thoi_gian/tat_bom/hour");
                refHourOff.set(widget.timerSetting.timerEnd.hour);

                DatabaseReference refMinOff = FirebaseDatabase.instance.ref("device/bom_tuoi_cay/thoi_gian/tat_bom/min");
                refMinOff.set(widget.timerSetting.timerEnd.minute);

                ScaffoldMessenger.of(context).showSnackBar(SnackBar(
                backgroundColor: Colors.green,
                content: Text(
                  "Đã hẹn giờ tưới cây !",
                  style: TextStyle(fontSize: 18.0),
                )));
              }

              if (widget.timerSetting.settingTitle == "Hẹn giờ bật/tắt đèn") {

                DatabaseReference refHourOn = FirebaseDatabase.instance.ref("device/den_quang_hop/thoi_gian/bat_den/hour");
                refHourOn.set(widget.timerSetting.timerBegin.hour);

                DatabaseReference refMinOn = FirebaseDatabase.instance.ref("device/den_quang_hop/thoi_gian/bat_den/min");
                refMinOn.set(widget.timerSetting.timerBegin.minute);

                DatabaseReference refHourOff = FirebaseDatabase.instance.ref("device/den_quang_hop/thoi_gian/tat_den/hour");
                refHourOff.set(widget.timerSetting.timerEnd.hour);

                DatabaseReference refMinOff = FirebaseDatabase.instance.ref("device/den_quang_hop/thoi_gian/tat_den/min");
                refMinOff.set(widget.timerSetting.timerEnd.minute);

                ScaffoldMessenger.of(context).showSnackBar(SnackBar(
                backgroundColor: Colors.green,
                content: Text(
                  "Đã hẹn giờ cho đèn quang hợp !",
                  style: TextStyle(fontSize: 18.0),
                )));
              }


              if (widget.timerSetting.settingTitle == "Hẹn giờ đóng/mở mái che") {

                DatabaseReference refHourOn = FirebaseDatabase.instance.ref("device/mai_che/thoi_gian/dong_mai_che/hour");
                refHourOn.set(widget.timerSetting.timerBegin.hour);

                DatabaseReference refMinOn = FirebaseDatabase.instance.ref("device/mai_che/thoi_gian/dong_mai_che/min");
                refMinOn.set(widget.timerSetting.timerBegin.minute);

                DatabaseReference refHourOff = FirebaseDatabase.instance.ref("device/mai_che/thoi_gian/mo_mai_che/hour");
                refHourOff.set(widget.timerSetting.timerEnd.hour);

                DatabaseReference refMinOff = FirebaseDatabase.instance.ref("device/mai_che/thoi_gian/mo_mai_che/min");
                refMinOff.set(widget.timerSetting.timerEnd.minute);

                ScaffoldMessenger.of(context).showSnackBar(SnackBar(
                backgroundColor: Colors.green,
                content: Text(
                  "Đã hẹn giờ cho mái che !",
                  style: TextStyle(fontSize: 18.0),
                )));
              }


              if (widget.timerSetting.settingTitle == "Hẹn giờ bật/tắt phun") {

                DatabaseReference refHourOn = FirebaseDatabase.instance.ref("device/phun_suong/thoi_gian/bat_phun/hour");
                refHourOn.set(widget.timerSetting.timerBegin.hour);

                DatabaseReference refMinOn = FirebaseDatabase.instance.ref("device/phun_suong/thoi_gian/bat_phun/min");
                refMinOn.set(widget.timerSetting.timerBegin.minute);

                DatabaseReference refHourOff = FirebaseDatabase.instance.ref("device/phun_suong/thoi_gian/tat_phun/hour");
                refHourOff.set(widget.timerSetting.timerEnd.hour);

                DatabaseReference refMinOff = FirebaseDatabase.instance.ref("device/phun_suong/thoi_gian/tat_phun/min");
                refMinOff.set(widget.timerSetting.timerEnd.minute);

                ScaffoldMessenger.of(context).showSnackBar(SnackBar(
                backgroundColor: Colors.green,
                content: Text(
                  "Đã hẹn giờ cho phun sương !",
                  style: TextStyle(fontSize: 18.0),
                )));
              }



            }, child: Text("Lưu"))
            : SizedBox(),
      ],
    );
  }
}
