#include "include/web_gpu/web_gpu_plugin.h"

#include <dawn/dawn_proc.h>
#include <dawn_native/DawnNative.h>
#include <flutter_linux/flutter_linux.h>

#define WEB_GPU_PLUGIN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST((obj), web_gpu_plugin_get_type(), WebGpuPlugin))

struct _WebGpuPlugin {
  GObject parent_instance;
};

G_DEFINE_TYPE(WebGpuPlugin, web_gpu_plugin, g_object_get_type())

// Called when a method call is received from Flutter.
static void web_gpu_plugin_handle_method_call(WebGpuPlugin* self,
                                              FlMethodCall* method_call) {
  g_autoptr(FlMethodResponse) response = nullptr;

  const gchar* method = fl_method_call_get_name(method_call);

  if (strcmp(method, "init") == 0) {
    dawnProcSetProcs(&dawn_native::GetProcs());
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  fl_method_call_respond(method_call, response, nullptr);
}

static void web_gpu_plugin_dispose(GObject* object) {
  G_OBJECT_CLASS(web_gpu_plugin_parent_class)->dispose(object);
}

static void web_gpu_plugin_class_init(WebGpuPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = web_gpu_plugin_dispose;
}

static void web_gpu_plugin_init(WebGpuPlugin* self) {}

static void method_call_cb(FlMethodChannel* channel, FlMethodCall* method_call,
                           gpointer user_data) {
  WebGpuPlugin* plugin = WEB_GPU_PLUGIN(user_data);
  web_gpu_plugin_handle_method_call(plugin, method_call);
}

void web_gpu_plugin_register_with_registrar(FlPluginRegistrar* registrar) {
  WebGpuPlugin* plugin =
      WEB_GPU_PLUGIN(g_object_new(web_gpu_plugin_get_type(), nullptr));
  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  g_autoptr(FlMethodChannel) channel =
      fl_method_channel_new(fl_plugin_registrar_get_messenger(registrar),
                            "web_gpu", FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(
      channel, method_call_cb, g_object_ref(plugin), g_object_unref);

  g_object_unref(plugin);
}
