import 'package:flutter/material.dart';

class Guide extends StatelessWidget {
const Guide({ Key? key }) : super(key: key);

  @override
  Widget build(BuildContext context){
    return Scaffold(
      appBar: AppBar(
        centerTitle: true,
        title: const Text('Hướng dẫn'),
      ),
      body: Container(),
    );
  }
}