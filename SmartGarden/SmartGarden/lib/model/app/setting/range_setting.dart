class RangeSetting {
  String settingTitle;
  int rangeValue; // không cần max - min vì thực tế nhỏ hơn ngưỡng này bơm sẽ bật
  
  RangeSetting({required this.rangeValue, required this.settingTitle});
}