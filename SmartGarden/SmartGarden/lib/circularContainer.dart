import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';

class CircularContainer extends StatefulWidget {
  Color color;
  CircularContainer({ super.key, required this.color });

  @override
  _CircularContainerState createState() => _CircularContainerState();
}

class _CircularContainerState extends State<CircularContainer> {
  @override
  Widget build(BuildContext context) {
    return Container(
      margin: EdgeInsets.all(4),
      width: 20,
      height: 6,
      decoration: BoxDecoration(
        color: widget.color,
        borderRadius: BorderRadius.circular(20)
      ),
    );
  }
}