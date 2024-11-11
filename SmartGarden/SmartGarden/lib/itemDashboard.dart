import 'dart:async';
import 'dart:ui';

import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:flutter_application_1/itemDashboardDetail.dart';
import 'package:flutter_application_1/model/app/content/item.dart';
import 'package:flutter_application_1/model/app/content/light.dart';
import 'package:flutter_application_1/model/app/content/mist_spray.dart';
import 'package:flutter_application_1/model/app/content/roof.dart';
import 'package:flutter_application_1/model/app/content/watering.dart';


class ItemDashboard extends StatefulWidget {
  Item item;

  ItemDashboard(
      {super.key,required this.item});

  @override
  _ItemDashboardState createState() => _ItemDashboardState();
}

class _ItemDashboardState extends State<ItemDashboard> {

  late DatabaseReference _deviceRef;
  late StreamSubscription<DatabaseEvent> _deviceSubscription;


  @override
  void initState() {
    // TODO: implement initState
    super.initState();
    init();
    print("init state con !!");

  }

  Future<void> init() async {
    _deviceRef  = FirebaseDatabase.instance.ref("device");

    // setState(() {
    //   initialized = true;
    // });

    _deviceSubscription = _deviceRef.onValue.listen(
      (DatabaseEvent event) {
        setState(() {
          // _error = null;

          final value = event.snapshot.value;
          final device = (value as dynamic)['device'];

          if (value != null) {
            // Map<dynamic, dynamic> dataConvert = value as Map<dynamic, dynamic>;

            if (widget.item is Watering) {
              (widget.item as Watering).isOnDevice = (value as dynamic)['bom_tuoi_cay']['nguon'];
              (widget.item as Watering).isAuto = (value as dynamic)['bom_tuoi_cay']['auto'];
              (widget.item as Watering).haveATimer = (value as dynamic)['bom_tuoi_cay']['hen_gio'];
            }

            if (widget.item is Light) {
              (widget.item as Light).isOnDevice = (value as dynamic)['den_quang_hop']['nguon'];
              (widget.item as Light).isAuto = (value as dynamic)['den_quang_hop']['auto'];
              (widget.item as Light).haveATimer = (value as dynamic)['den_quang_hop']['hen_gio'];
            } 

            if (widget.item is Roof) {
              (widget.item as Roof).isOnDevice = (value as dynamic)['mai_che']['nguon'];
              (widget.item as Roof).isAuto = (value as dynamic)['mai_che']['auto'];
              (widget.item as Roof).haveATimer = (value as dynamic)['mai_che']['hen_gio'];
            }

            if (widget.item is MistSpray) {
              (widget.item as MistSpray).isOnDevice = (value as dynamic)['phun_suong']['nguon'];
              (widget.item as MistSpray).isAuto = (value as dynamic)['phun_suong']['auto'];
              (widget.item as MistSpray).haveATimer = (value as dynamic)['phun_suong']['hen_gio'];
            }
          }
        });
      },

      // onError: (Object o) {
      //   final error = o as FirebaseException;
      //   setState(() {
      //     _error = error;
      //   });
      // },
    );
   
  }


  @override
  void dispose() {
    // TODO: implement dispose
    super.dispose();
    _deviceSubscription.cancel();
    print("con bị hủy !");
  }

  @override
  Widget build(BuildContext context) {
    return GestureDetector(
      onTap: () {
        Navigator.push(
            context,
            MaterialPageRoute(
                builder: (context) => ItemDashboardDetail(
                      item: widget.item,
                    )));
      },
      child: Container(
        padding: EdgeInsets.all(10),
        decoration: BoxDecoration(
            color: Colors.white,
            borderRadius: BorderRadius.circular(14),
            boxShadow: [
              BoxShadow(
                  offset: const Offset(0, 5),
                  color: Theme.of(context).primaryColor.withOpacity(.2),
                  spreadRadius: 2,
                  blurRadius: 5)
            ]),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          // mainAxisAlignment: MainAxisAlignment.center,
          children: [
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Container(
                  width: 46,
                  child: Image.asset(widget.item.iconPath),
                ),
                widget.item.isAuto ? Text("Auto", style: TextStyle(color: Colors.green, fontWeight: FontWeight.w600),) : SizedBox(),
                widget.item.haveATimer ? Icon(
                      color: Colors.green,
                      CupertinoIcons.clock,
                      size: 18,
                    ) : Container()
                
              ]
            ),
            Container(
              width: double.maxFinite,
              child: Text(
                widget.item.title,
                style: Theme.of(context).textTheme.titleMedium,
              ),
            ),
            Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Text(
                  widget.item.isOnDevice ? "Đang bật" : "Đang tắt",
                  style: Theme.of(context).textTheme.labelMedium,
                ),
                Switch(
                    value: widget.item.isOnDevice,
                    onChanged: widget.item.isAuto ? null : (bool value) {
                      if (widget.item is Watering) {
                          DatabaseReference ref = FirebaseDatabase.instance.ref("device/bom_tuoi_cay/nguon");
                          if (value) {
                              ref.set(true);
                          } else {
                            ref.set(false);
                          }
                      }

                      if (widget.item is Light) {
                          DatabaseReference ref = FirebaseDatabase.instance.ref("device/den_quang_hop/nguon");
                          if (value) {
                              ref.set(true);
                          } else {
                            ref.set(false);
                          }
                      }

                      if (widget.item is MistSpray) {
                          DatabaseReference ref = FirebaseDatabase.instance.ref("device/phun_suong/nguon");
                          if (value) {
                              ref.set(true);
                          } else {
                            ref.set(false);
                          }
                      }

                      if (widget.item is Roof) {
                          DatabaseReference ref = FirebaseDatabase.instance.ref("device/mai_che/nguon");
                          if (value) {
                              ref.set(true);
                          } else {
                            ref.set(false);
                          }
                      }

                      setState(() {
                        widget.item.isOnDevice = value;
                      });
                    })
              ],
            )
          ],
        ),
      ),
    );
  }
}
