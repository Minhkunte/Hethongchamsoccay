import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:flutter_application_1/model/app/setting/level_setting.dart';

class LevelSettingContainer extends StatefulWidget {
  LevelSetting levelSetting;
  LevelSettingContainer({super.key, required this.levelSetting});

  @override
  _LevelSettingContainerState createState() => _LevelSettingContainerState();
}

class _LevelSettingContainerState extends State<LevelSettingContainer> {
  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Text(widget.levelSetting.settingTitle),
        SizedBox(
          height: 94,
        ),
        Row(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Column(
              children: [
                Text(widget.levelSetting.lowLevelLabel),
                Radio(
                    value: 1, 
                    groupValue: widget.levelSetting.value,
                    onChanged: (value) {
                      setState(() {
                        widget.levelSetting.value = value!;
                      });
                    }),
              ],
            ),
            SizedBox(width: 20,),
            Column(
              children: [
                Text(widget.levelSetting.mediumLevelLabel),
                Radio(
                    value: 2,
                    groupValue: widget.levelSetting.value,
                    onChanged: (value) {
                      setState(() {
                        widget.levelSetting.value = value!;
                      });
                    }),
              ],
            ),
            SizedBox(width: 20,),
            Column(
              children: [
                Text(widget.levelSetting.highLevelLabel),
                Radio(
                    value: 3,
                    groupValue: widget.levelSetting.value,
                    onChanged: (value) {
                      setState(() {
                        widget.levelSetting.value = value!;
                      });
                    }),
              ],
            ),
          ],
        ), // Hiển thị giá trị tối đa
        SizedBox(
          height: 20,
        ),
        FilledButton(onPressed: () {
          if (widget.levelSetting.settingTitle == "Tốc độ bơm") {
             DatabaseReference ref = FirebaseDatabase.instance.ref("device/bom_tuoi_cay/toc_do_bom");
                          ref.set(widget.levelSetting.value);
            ScaffoldMessenger.of(context).showSnackBar(SnackBar(
            backgroundColor: Colors.green,
            content: Text(
              "Thay đổi tốc độ bơm thành công !",
              style: TextStyle(fontSize: 18.0),
            )));
          }

          if (widget.levelSetting.settingTitle == "Thiết lập độ nhạy") {
            DatabaseReference ref = FirebaseDatabase.instance.ref("device/den_quang_hop/do_nhay");
                          ref.set(widget.levelSetting.value);
            ScaffoldMessenger.of(context).showSnackBar(SnackBar(
            backgroundColor: Colors.green,
            content: Text(
              "Thay đổi độ nhạy thành công !",
              style: TextStyle(fontSize: 18.0),
            )));
          }

          if (widget.levelSetting.settingTitle == "Tốc độ phun") {
            DatabaseReference ref = FirebaseDatabase.instance.ref("device/phun_suong/toc_do_phun");
                          ref.set(widget.levelSetting.value);
            ScaffoldMessenger.of(context).showSnackBar(SnackBar(
            backgroundColor: Colors.green,
            content: Text(
              "Thay đổi tốc độ phun thành công !",
              style: TextStyle(fontSize: 18.0),
            )));
          }
          
        }, child: Text("Lưu")),
      ],
    );
  }
}
