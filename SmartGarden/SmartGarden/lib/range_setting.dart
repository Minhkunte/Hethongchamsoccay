import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_application_1/model/app/setting/range_setting.dart';

import 'model/app/content/item.dart';

class RangeSettingContainer extends StatefulWidget {
  RangeSetting rangeSetting;
  RangeSettingContainer({Key? key, required this.rangeSetting}) : super(key: key);

  @override
  _RangeSettingContainerState createState() => _RangeSettingContainerState();
}

class _RangeSettingContainerState extends State<RangeSettingContainer> {


  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    init();
  }

  Future<void> init() async {
    final ref = FirebaseDatabase.instance.ref("device");
    final snapshot = await ref.get();
    if (snapshot.exists) {
        if (widget.rangeSetting.settingTitle == "Thiết lập ngưỡng độ ẩm không khí") {
          widget.rangeSetting.rangeValue = (snapshot.value as dynamic)['phun_suong']['nguong_do_am_mt'];
        }

        if (widget.rangeSetting.settingTitle == "Thiết lập ngưỡng độ ẩm đất") {
          widget.rangeSetting.rangeValue = (snapshot.value as dynamic)['bom_tuoi_cay']['nguong_do_am_dat'];
        }
    } else {
        print('No data available.');
    }
  }

  @override
  Widget build(BuildContext context) {
    return Column(
      children: [
        Text(widget.rangeSetting.settingTitle),
        SizedBox(height: 72,),
        Text("${widget.rangeSetting.rangeValue}%"),
         // Hiển thị giá trị tối đa
        Slider(
          value: widget.rangeSetting.rangeValue.toDouble(),
          min: 0,
          max: 100,
          onChanged: (value) {
            setState(() {
              widget.rangeSetting.rangeValue = value.ceil();
            });
          },
        ),
        
        SizedBox(
          height: 40,
        ),
        FilledButton(onPressed: () {
          if (widget.rangeSetting.settingTitle == "Thiết lập ngưỡng độ ẩm đất") {

                DatabaseReference ref = FirebaseDatabase.instance.ref("device/bom_tuoi_cay/nguong_do_am_dat");
                              ref.set(widget.rangeSetting.rangeValue);
                ScaffoldMessenger.of(context).showSnackBar(SnackBar(
                backgroundColor: Colors.green,
                content: Text(
                  "Đã thiết lập ngưỡng nhiệt độ. Khi độ ẩm đất dưới ${widget.rangeSetting.rangeValue}% bơm tưới cây sẽ bật !",
                  style: TextStyle(fontSize: 18.0),
                )));
          }

          if (widget.rangeSetting.settingTitle == "Thiết lập ngưỡng độ ẩm không khí") {
            DatabaseReference ref = FirebaseDatabase.instance.ref("device/phun_suong/nguong_do_am_mt");
                          ref.set(widget.rangeSetting.rangeValue);
            ScaffoldMessenger.of(context).showSnackBar(SnackBar(
            backgroundColor: Colors.green,
            content: Text(
              "Đã thiết lập ngưỡng nhiệt độ. Khi độ ẩm không khí dưới ${widget.rangeSetting.rangeValue}% phun sương sẽ bật !",
              style: TextStyle(fontSize: 18.0),
            )));
          }


        }, child: Text("Lưu")),
      ],
    );
  }
}

