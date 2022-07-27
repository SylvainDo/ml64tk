#include <js_native_api.h>
#include <node_api.h>
#include <dlfcn.h>
#include "symbols.h"

#define X(x) decltype(&x) pfn_##x;
SYMBOLS
#undef X
NAPI_NO_RETURN decltype(&napi_fatal_error) pfn_napi_fatal_error;

struct Loader {
    Loader() {
#define X(x) pfn_##x = reinterpret_cast<decltype(&x)>(dlsym(RTLD_DEFAULT, #x));
SYMBOLS
#undef X
        pfn_napi_fatal_error = reinterpret_cast<decltype(&napi_fatal_error)>(dlsym(RTLD_DEFAULT, "napi_fatal_error"));
    }
} loader;

napi_status napi_get_last_error_info(napi_env env, const napi_extended_error_info** result) { return pfn_napi_get_last_error_info(env, result); }
napi_status napi_get_undefined(napi_env env, napi_value* result) { return pfn_napi_get_undefined(env, result); }
napi_status napi_get_null(napi_env env, napi_value* result) { return pfn_napi_get_null(env, result); }
napi_status napi_get_global(napi_env env, napi_value* result) { return pfn_napi_get_global(env, result); }
napi_status napi_get_boolean(napi_env env, bool value, napi_value* result) { return pfn_napi_get_boolean(env, value, result); }
napi_status napi_create_object(napi_env env, napi_value* result) { return pfn_napi_create_object(env, result); }
napi_status napi_create_array(napi_env env, napi_value* result) { return pfn_napi_create_array(env, result); }
napi_status napi_create_array_with_length(napi_env env, size_t length, napi_value* result) { return pfn_napi_create_array_with_length(env, length, result); }
napi_status napi_create_double(napi_env env, double value, napi_value* result) { return pfn_napi_create_double(env, value, result); }
napi_status napi_create_int32(napi_env env, int32_t value, napi_value* result) { return pfn_napi_create_int32(env, value, result); }
napi_status napi_create_uint32(napi_env env, uint32_t value, napi_value* result) { return pfn_napi_create_uint32(env, value, result); }
napi_status napi_create_int64(napi_env env, int64_t value, napi_value* result) { return pfn_napi_create_int64(env, value, result); }
napi_status napi_create_string_latin1(napi_env env, const char* str, size_t length, napi_value* result) { return pfn_napi_create_string_latin1(env, str, length, result); }
napi_status napi_create_string_utf8(napi_env env, const char* str, size_t length, napi_value* result) { return pfn_napi_create_string_utf8(env, str, length, result); }
napi_status napi_create_string_utf16(napi_env env, const char16_t* str, size_t length, napi_value* result) { return pfn_napi_create_string_utf16(env, str, length, result); }
napi_status napi_create_symbol(napi_env env, napi_value description, napi_value* result) { return pfn_napi_create_symbol(env, description, result); }
napi_status napi_create_function(napi_env env, const char* utf8name, size_t length, napi_callback cb, void* data, napi_value* result) { return pfn_napi_create_function(env, utf8name, length, cb, data, result); }
napi_status napi_create_error(napi_env env, napi_value code, napi_value msg, napi_value* result) { return pfn_napi_create_error(env, code, msg, result); }
napi_status napi_create_type_error(napi_env env, napi_value code, napi_value msg, napi_value* result) { return pfn_napi_create_type_error(env, code, msg, result); }
napi_status napi_create_range_error(napi_env env, napi_value code, napi_value msg, napi_value* result) { return pfn_napi_create_range_error(env, code, msg, result); }
napi_status napi_typeof(napi_env env, napi_value value, napi_valuetype* result) { return pfn_napi_typeof(env, value, result); }
napi_status napi_get_value_double(napi_env env, napi_value value, double* result) { return pfn_napi_get_value_double(env, value, result); }
napi_status napi_get_value_int32(napi_env env, napi_value value, int32_t* result) { return pfn_napi_get_value_int32(env, value, result); }
napi_status napi_get_value_uint32(napi_env env, napi_value value, uint32_t* result) { return pfn_napi_get_value_uint32(env, value, result); }
napi_status napi_get_value_int64(napi_env env, napi_value value, int64_t* result) { return pfn_napi_get_value_int64(env, value, result); }
napi_status napi_get_value_bool(napi_env env, napi_value value, bool* result) { return pfn_napi_get_value_bool(env, value, result); }
napi_status napi_get_value_string_latin1(napi_env env, napi_value value, char* buf, size_t bufsize, size_t* result) { return pfn_napi_get_value_string_latin1(env, value, buf, bufsize, result); }
napi_status napi_get_value_string_utf8(napi_env env, napi_value value, char* buf, size_t bufsize, size_t* result) { return pfn_napi_get_value_string_utf8(env, value, buf, bufsize, result); }
napi_status napi_get_value_string_utf16(napi_env env, napi_value value, char16_t* buf, size_t bufsize, size_t* result) { return pfn_napi_get_value_string_utf16(env, value, buf, bufsize, result); }
napi_status napi_coerce_to_bool(napi_env env, napi_value value, napi_value* result) { return pfn_napi_coerce_to_bool(env, value, result); }
napi_status napi_coerce_to_number(napi_env env, napi_value value, napi_value* result) { return pfn_napi_coerce_to_number(env, value, result); }
napi_status napi_coerce_to_object(napi_env env, napi_value value, napi_value* result) { return pfn_napi_coerce_to_object(env, value, result); }
napi_status napi_coerce_to_string(napi_env env, napi_value value, napi_value* result) { return pfn_napi_coerce_to_string(env, value, result); }
napi_status napi_get_prototype(napi_env env, napi_value object, napi_value* result) { return pfn_napi_get_prototype(env, object, result); }
napi_status napi_get_property_names(napi_env env, napi_value object, napi_value* result) { return pfn_napi_get_property_names(env, object, result); }
napi_status napi_set_property(napi_env env, napi_value object, napi_value key, napi_value value) { return pfn_napi_set_property(env, object, key, value); }
napi_status napi_has_property(napi_env env, napi_value object, napi_value key, bool* result) { return pfn_napi_has_property(env, object, key, result); }
napi_status napi_get_property(napi_env env, napi_value object, napi_value key, napi_value* result) { return pfn_napi_get_property(env, object, key, result); }
napi_status napi_delete_property(napi_env env, napi_value object, napi_value key, bool* result) { return pfn_napi_delete_property(env, object, key, result); }
napi_status napi_has_own_property(napi_env env, napi_value object, napi_value key, bool* result) { return pfn_napi_has_own_property(env, object, key, result); }
napi_status napi_set_named_property(napi_env env, napi_value object, const char* utf8name, napi_value value) { return pfn_napi_set_named_property(env, object, utf8name, value); }
napi_status napi_has_named_property(napi_env env, napi_value object, const char* utf8name, bool* result) { return pfn_napi_has_named_property(env, object, utf8name, result); }
napi_status napi_get_named_property(napi_env env, napi_value object, const char* utf8name, napi_value* result) { return pfn_napi_get_named_property(env, object, utf8name, result); }
napi_status napi_set_element(napi_env env, napi_value object, uint32_t index, napi_value value) { return pfn_napi_set_element(env, object, index, value); }
napi_status napi_has_element(napi_env env, napi_value object, uint32_t index, bool* result) { return pfn_napi_has_element(env, object, index, result); }
napi_status napi_get_element(napi_env env, napi_value object, uint32_t index, napi_value* result) { return pfn_napi_get_element(env, object, index, result); }
napi_status napi_delete_element(napi_env env, napi_value object, uint32_t index, bool* result) { return pfn_napi_delete_element(env, object, index, result); }
napi_status napi_define_properties(napi_env env, napi_value object, size_t property_count, const napi_property_descriptor* properties) { return pfn_napi_define_properties(env, object, property_count, properties); }
napi_status napi_is_array(napi_env env, napi_value value, bool* result) { return pfn_napi_is_array(env, value, result); }
napi_status napi_get_array_length(napi_env env, napi_value value, uint32_t* result) { return pfn_napi_get_array_length(env, value, result); }
napi_status napi_strict_equals(napi_env env, napi_value lhs, napi_value rhs, bool* result) { return pfn_napi_strict_equals(env, lhs, rhs, result); }
napi_status napi_call_function(napi_env env, napi_value recv, napi_value func, size_t argc, const napi_value* argv, napi_value* result) { return pfn_napi_call_function(env, recv, func, argc, argv, result); }
napi_status napi_new_instance(napi_env env, napi_value constructor, size_t argc, const napi_value* argv, napi_value* result) { return pfn_napi_new_instance(env, constructor, argc, argv, result); }
napi_status napi_instanceof(napi_env env, napi_value object, napi_value constructor, bool* result) { return pfn_napi_instanceof(env, object, constructor, result); }
napi_status napi_get_cb_info(napi_env env, napi_callback_info cbinfo, size_t* argc, napi_value* argv, napi_value* this_arg, void** data) { return pfn_napi_get_cb_info(env, cbinfo, argc, argv, this_arg, data); }
napi_status napi_get_new_target(napi_env env, napi_callback_info cbinfo, napi_value* result) { return pfn_napi_get_new_target(env, cbinfo, result); }
napi_status napi_define_class(napi_env env, const char* utf8name, size_t length, napi_callback constructor, void* data, size_t property_count, const napi_property_descriptor* properties, napi_value* result) { return pfn_napi_define_class(env, utf8name, length, constructor, data, property_count, properties, result); }
napi_status napi_wrap(napi_env env, napi_value js_object, void* native_object, napi_finalize finalize_cb, void* finalize_hint, napi_ref* result) { return pfn_napi_wrap(env, js_object, native_object, finalize_cb, finalize_hint, result); }
napi_status napi_unwrap(napi_env env, napi_value js_object, void** result) { return pfn_napi_unwrap(env, js_object, result); }
napi_status napi_remove_wrap(napi_env env, napi_value js_object, void** result) { return pfn_napi_remove_wrap(env, js_object, result); }
napi_status napi_create_external(napi_env env, void* data, napi_finalize finalize_cb, void* finalize_hint, napi_value* result) { return pfn_napi_create_external(env, data, finalize_cb, finalize_hint, result); }
napi_status napi_get_value_external(napi_env env, napi_value value, void** result) { return pfn_napi_get_value_external(env, value, result); }
napi_status napi_create_reference(napi_env env, napi_value value, uint32_t initial_refcount, napi_ref* result) { return pfn_napi_create_reference(env, value, initial_refcount, result); }
napi_status napi_delete_reference(napi_env env, napi_ref ref) { return pfn_napi_delete_reference(env, ref); }
napi_status napi_reference_ref(napi_env env, napi_ref ref, uint32_t* result) { return pfn_napi_reference_ref(env, ref, result); }
napi_status napi_reference_unref(napi_env env, napi_ref ref, uint32_t* result) { return pfn_napi_reference_unref(env, ref, result); }
napi_status napi_get_reference_value(napi_env env, napi_ref ref, napi_value* result) { return pfn_napi_get_reference_value(env, ref, result); }
napi_status napi_open_handle_scope(napi_env env, napi_handle_scope* result) { return pfn_napi_open_handle_scope(env, result); }
napi_status napi_close_handle_scope(napi_env env, napi_handle_scope scope) { return pfn_napi_close_handle_scope(env, scope); }
napi_status napi_open_escapable_handle_scope(napi_env env, napi_escapable_handle_scope* result) { return pfn_napi_open_escapable_handle_scope(env, result); }
napi_status napi_close_escapable_handle_scope(napi_env env, napi_escapable_handle_scope scope) { return pfn_napi_close_escapable_handle_scope(env, scope); }
napi_status napi_escape_handle(napi_env env, napi_escapable_handle_scope scope, napi_value escapee, napi_value* result) { return pfn_napi_escape_handle(env, scope, escapee, result); }
napi_status napi_throw(napi_env env, napi_value error) { return pfn_napi_throw(env, error); }
napi_status napi_throw_error(napi_env env, const char* code, const char* msg) { return pfn_napi_throw_error(env, code, msg); }
napi_status napi_throw_type_error(napi_env env, const char* code, const char* msg) { return pfn_napi_throw_type_error(env, code, msg); }
napi_status napi_throw_range_error(napi_env env, const char* code, const char* msg) { return pfn_napi_throw_range_error(env, code, msg); }
napi_status napi_is_error(napi_env env, napi_value value, bool* result) { return pfn_napi_is_error(env, value, result); }
napi_status napi_is_exception_pending(napi_env env, bool* result) { return pfn_napi_is_exception_pending(env, result); }
napi_status napi_get_and_clear_last_exception(napi_env env, napi_value* result) { return pfn_napi_get_and_clear_last_exception(env, result); }
napi_status napi_is_arraybuffer(napi_env env, napi_value value, bool* result) { return pfn_napi_is_arraybuffer(env, value, result); }
napi_status napi_create_arraybuffer(napi_env env, size_t byte_length, void** data, napi_value* result) { return pfn_napi_create_arraybuffer(env, byte_length, data, result); }
napi_status napi_create_external_arraybuffer(napi_env env, void* external_data, size_t byte_length, napi_finalize finalize_cb, void* finalize_hint, napi_value* result) { return pfn_napi_create_external_arraybuffer(env, external_data, byte_length, finalize_cb, finalize_hint, result); }
napi_status napi_get_arraybuffer_info(napi_env env, napi_value arraybuffer, void** data, size_t* byte_length) { return pfn_napi_get_arraybuffer_info(env, arraybuffer, data, byte_length); }
napi_status napi_is_typedarray(napi_env env, napi_value value, bool* result) { return pfn_napi_is_typedarray(env, value, result); }
napi_status napi_create_typedarray(napi_env env, napi_typedarray_type type, size_t length, napi_value arraybuffer, size_t byte_offset, napi_value* result) { return pfn_napi_create_typedarray(env, type, length, arraybuffer, byte_offset, result); }
napi_status napi_get_typedarray_info(napi_env env, napi_value typedarray, napi_typedarray_type* type, size_t* length, void** data, napi_value* arraybuffer, size_t* byte_offset) { return pfn_napi_get_typedarray_info(env, typedarray, type, length, data, arraybuffer, byte_offset); }
napi_status napi_create_dataview(napi_env env, size_t length, napi_value arraybuffer, size_t byte_offset, napi_value* result) { return pfn_napi_create_dataview(env, length, arraybuffer, byte_offset, result); }
napi_status napi_is_dataview(napi_env env, napi_value value, bool* result) { return pfn_napi_is_dataview(env, value, result); }
napi_status napi_get_dataview_info(napi_env env, napi_value dataview, size_t* bytelength, void** data, napi_value* arraybuffer, size_t* byte_offset) { return pfn_napi_get_dataview_info(env, dataview, bytelength, data, arraybuffer, byte_offset); }
napi_status napi_get_version(napi_env env, uint32_t* result) { return napi_get_version(env, result); }
napi_status napi_create_promise(napi_env env, napi_deferred* deferred, napi_value* promise) { return pfn_napi_create_promise(env, deferred, promise); }
napi_status napi_resolve_deferred(napi_env env, napi_deferred deferred, napi_value resolution) { return pfn_napi_resolve_deferred(env, deferred, resolution); }
napi_status napi_reject_deferred(napi_env env, napi_deferred deferred, napi_value rejection) { return pfn_napi_reject_deferred(env, deferred, rejection); }
napi_status napi_is_promise(napi_env env, napi_value value, bool* is_promise) { return pfn_napi_is_promise(env, value, is_promise); }
napi_status napi_run_script(napi_env env, napi_value script, napi_value* result) { return pfn_napi_run_script(env, script, result); }
napi_status napi_adjust_external_memory(napi_env env, int64_t change_in_bytes, int64_t* adjusted_value) { return pfn_napi_adjust_external_memory(env, change_in_bytes, adjusted_value); }
napi_status napi_create_date(napi_env env, double time, napi_value* result) { return pfn_napi_create_date(env, time, result); }
napi_status napi_is_date(napi_env env, napi_value value, bool* is_date) { return pfn_napi_is_date(env, value, is_date); }
napi_status napi_get_date_value(napi_env env, napi_value value, double* result) { return pfn_napi_get_date_value(env, value, result); }
napi_status napi_add_finalizer(napi_env env, napi_value js_object, void* native_object, napi_finalize finalize_cb, void* finalize_hint, napi_ref* result) { return pfn_napi_add_finalizer(env, js_object, native_object, finalize_cb, finalize_hint, result); }
napi_status napi_create_bigint_int64(napi_env env, int64_t value, napi_value* result) { return pfn_napi_create_bigint_int64(env, value, result); }
napi_status napi_create_bigint_uint64(napi_env env, uint64_t value, napi_value* result) { return pfn_napi_create_bigint_uint64(env, value, result); }
napi_status napi_create_bigint_words(napi_env env, int sign_bit, size_t word_count, const uint64_t* words, napi_value* result) { return pfn_napi_create_bigint_words(env, sign_bit, word_count, words, result); }
napi_status napi_get_value_bigint_int64(napi_env env, napi_value value, int64_t* result, bool* lossless) { return pfn_napi_get_value_bigint_int64(env, value, result, lossless); }
napi_status napi_get_value_bigint_uint64(napi_env env, napi_value value, uint64_t* result, bool* lossless) { return pfn_napi_get_value_bigint_uint64(env, value, result, lossless); }
napi_status napi_get_value_bigint_words(napi_env env, napi_value value, int* sign_bit, size_t* word_count, uint64_t* words) { return pfn_napi_get_value_bigint_words(env, value, sign_bit, word_count, words); }
napi_status napi_get_all_property_names(napi_env env, napi_value object, napi_key_collection_mode key_mode, napi_key_filter key_filter, napi_key_conversion key_conversion, napi_value* result) { return pfn_napi_get_all_property_names(env, object, key_mode, key_filter, key_conversion, result); }
napi_status napi_set_instance_data(napi_env env, void* data, napi_finalize finalize_cb, void* finalize_hint) { return pfn_napi_set_instance_data(env, data, finalize_cb, finalize_hint); }
napi_status napi_get_instance_data(napi_env env, void** data) { return pfn_napi_get_instance_data(env, data); }
napi_status napi_detach_arraybuffer(napi_env env, napi_value arraybuffer) { return pfn_napi_detach_arraybuffer(env, arraybuffer); }
napi_status napi_is_detached_arraybuffer(napi_env env, napi_value value, bool* result) { return pfn_napi_is_detached_arraybuffer(env, value, result); }
napi_status napi_type_tag_object(napi_env env, napi_value value, const napi_type_tag* type_tag) { return pfn_napi_type_tag_object(env, value, type_tag); }
napi_status napi_check_object_type_tag(napi_env env, napi_value value, const napi_type_tag* type_tag, bool* result) { return pfn_napi_check_object_type_tag(env, value, type_tag, result); }
napi_status napi_object_freeze(napi_env env, napi_value object) { return pfn_napi_object_freeze(env, object); }
napi_status napi_object_seal(napi_env env, napi_value object) { return pfn_napi_object_seal(env, object); }

void napi_module_register(napi_module* mod) { return pfn_napi_module_register(mod); }
NAPI_NO_RETURN void napi_fatal_error(const char* location, size_t location_len, const char* message, size_t message_len) { pfn_napi_fatal_error(location, location_len, message, message_len); }
napi_status napi_async_init(napi_env env, napi_value async_resource, napi_value async_resource_name, napi_async_context* result) { return pfn_napi_async_init(env, async_resource, async_resource_name, result); }
napi_status napi_async_destroy(napi_env env, napi_async_context async_context) { return pfn_napi_async_destroy(env, async_context); }
napi_status napi_make_callback(napi_env env, napi_async_context async_context, napi_value recv, napi_value func, size_t argc, const napi_value* argv, napi_value* result) { return pfn_napi_make_callback(env, async_context, recv, func, argc, argv, result); }
napi_status napi_create_buffer(napi_env env, size_t length, void** data, napi_value* result) { return pfn_napi_create_buffer(env, length, data, result); }
napi_status napi_create_external_buffer(napi_env env, size_t length, void* data, napi_finalize finalize_cb, void* finalize_hint, napi_value* result) { return pfn_napi_create_external_buffer(env, length, data, finalize_cb, finalize_hint, result); }
napi_status napi_create_buffer_copy(napi_env env, size_t length, const void* data, void** result_data, napi_value* result) { return pfn_napi_create_buffer_copy(env, length, data, result_data, result); }
napi_status napi_is_buffer(napi_env env, napi_value value, bool* result) { return pfn_napi_is_buffer(env, value, result); }
napi_status napi_get_buffer_info(napi_env env, napi_value value, void** data, size_t* length) { return pfn_napi_get_buffer_info(env, value, data, length); }
napi_status napi_create_async_work(napi_env env, napi_value async_resource, napi_value async_resource_name, napi_async_execute_callback execute, napi_async_complete_callback complete, void* data, napi_async_work* result) { return pfn_napi_create_async_work(env, async_resource, async_resource_name, execute, complete, data, result); }
napi_status napi_delete_async_work(napi_env env, napi_async_work work) { return pfn_napi_delete_async_work(env, work); }
napi_status napi_queue_async_work(napi_env env, napi_async_work work) { return pfn_napi_queue_async_work(env, work); }
napi_status napi_cancel_async_work(napi_env env, napi_async_work work) { return pfn_napi_cancel_async_work(env, work); }
napi_status napi_get_node_version(napi_env env, const napi_node_version** version) { return pfn_napi_get_node_version(env, version); }
napi_status napi_get_uv_event_loop(napi_env env, struct uv_loop_s** loop) { return pfn_napi_get_uv_event_loop(env, loop); }
napi_status napi_fatal_exception(napi_env env, napi_value err) { return pfn_napi_fatal_exception(env, err); }
napi_status napi_add_env_cleanup_hook(napi_env env, void (*fun)(void* arg), void* arg) { return pfn_napi_add_env_cleanup_hook(env, fun, arg); }
napi_status napi_remove_env_cleanup_hook(napi_env env, void (*fun)(void* arg), void* arg) { return pfn_napi_remove_env_cleanup_hook(env, fun, arg); }
napi_status napi_open_callback_scope(napi_env env, napi_value resource_object, napi_async_context context, napi_callback_scope* result) { return pfn_napi_open_callback_scope(env, resource_object, context, result); }
napi_status napi_close_callback_scope(napi_env env, napi_callback_scope scope) { return pfn_napi_close_callback_scope(env, scope); }
napi_status napi_create_threadsafe_function(napi_env env, napi_value func, napi_value async_resource, napi_value async_resource_name, size_t max_queue_size, size_t initial_thread_count, void* thread_finalize_data, napi_finalize thread_finalize_cb, void* context, napi_threadsafe_function_call_js call_js_cb, napi_threadsafe_function* result) { return pfn_napi_create_threadsafe_function(env, func, async_resource, async_resource_name, max_queue_size, initial_thread_count, thread_finalize_data, thread_finalize_cb, context, call_js_cb, result); }
napi_status napi_get_threadsafe_function_context(napi_threadsafe_function func, void** result) { return pfn_napi_get_threadsafe_function_context(func, result); }
napi_status napi_call_threadsafe_function(napi_threadsafe_function func, void* data, napi_threadsafe_function_call_mode is_blocking) { return pfn_napi_call_threadsafe_function(func, data, is_blocking); }
napi_status napi_acquire_threadsafe_function(napi_threadsafe_function func) { return pfn_napi_acquire_threadsafe_function(func); }
napi_status napi_release_threadsafe_function(napi_threadsafe_function func, napi_threadsafe_function_release_mode mode) { return pfn_napi_release_threadsafe_function(func, mode); }
napi_status napi_unref_threadsafe_function(napi_env env, napi_threadsafe_function func) { return pfn_napi_unref_threadsafe_function(env, func); }
napi_status napi_ref_threadsafe_function(napi_env env, napi_threadsafe_function func) { return pfn_napi_ref_threadsafe_function(env, func); }
napi_status napi_add_async_cleanup_hook(napi_env env, napi_async_cleanup_hook hook, void* arg, napi_async_cleanup_hook_handle* remove_handle) { return pfn_napi_add_async_cleanup_hook(env, hook, arg, remove_handle); }
napi_status napi_remove_async_cleanup_hook(napi_async_cleanup_hook_handle remove_handle) { return pfn_napi_remove_async_cleanup_hook(remove_handle); }
