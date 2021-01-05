import 'dart:ffi' as ffi;

import 'dylib.dart';

typedef _c_initWebGpu = ffi.Void Function();
typedef _dart_initWebGpu = void Function();

bool initWebGpu() {
  final exe = ffi.DynamicLibrary.open(fixupLibName('web_gpu_plugin'));
  final initFunc =
      exe.lookupFunction<_c_initWebGpu, _dart_initWebGpu>('initWebGpu');
  if (initFunc == null) return false;
  initFunc();
  return true;
}
