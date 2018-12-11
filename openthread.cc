// hello.cc using N-API
#include <node_api.h>

namespace ot {
namespace nodejs {

#define METHOD_OPEN "open"

class OpenThread
{
    static napi_value init(napi_env env, napi_value exports)
    {
        napi_status status;
        napi_value  fn;

        status =
            napi_define_class(env, "OpenThread", NAPI_AUTO_LENGTH, napi_callback constructor, void *data,
                              size_t property_count, const napi_property_descriptor *properties, napi_value *result);

        status = napi_create_function(env, METHOD_OPEN, NAPI_AUTO_LENGTH, Open, nullptr, &fn);
        if (status != napi_ok)
            return nullptr;

        status = napi_set_named_property(env, exports, "open", fn);
        if (status != napi_ok)
            return nullptr;
        return exports;
    }

    static napi_value New(napi_env env, napi_callback_info args)
    {
        napi_status status;

        napi_value target;
        status = napi_get_new_target(env, info, &target);
        assert(status == napi_ok);

        bool is_constructor = target != nullptr;

        if (is_constructor)
        {
            // Invoked as constructor: `new MyObject(...)`
            size_t     argc = 2;
            napi_value args[2];
            napi_value jsthis;
            char       aRadioFile[255];
            char       aRadioConfig[255];

            status = napi_get_cb_info(env, info, &argc, args, &jsthis, nullptr);
            assert(status == napi_ok);

            napi_valuetype valuetype;
            status = napi_typeof(env, args[0], &valuetype);
            assert(status == napi_ok);

            if (valuetype != napi_string)
            {
                size_t count = 0;
                status       = napi_get_value_string_utf8(env, args[0], aRadioFile, sizeof(aRadioFile), &count);
                assert(status == napi_ok);
            }

            napi_valuetype valuetype;
            status = napi_typeof(env, args[1], &valuetype);
            assert(status == napi_ok);

            if (valuetype != napi_string)
            {
                size_t count = 0;
                status       = napi_get_value_string_utf8(env, args[1], aRadioConfig, sizeof(aRadioConfig), &count);
                assert(status == napi_ok);
            }

            OpenThread *obj = new OpenThread(aRadioFile, aRadioConfig);

            obj->env_ = env;
            status    = napi_wrap(env, jsthis, reinterpret_cast<void *>(obj), OpenThread::Destructor,
                               nullptr, // finalize_hint
                               &obj->wrapper_);
            assert(status == napi_ok);

            return jsthis;
        }
        else
        {
            // Invoked as plain function `MyObject(...)`, turn into construct call.
            size_t     argc_ = 1;
            napi_value args[1];
            status = napi_get_cb_info(env, info, &argc_, args, nullptr, nullptr);
            assert(status == napi_ok);

            const size_t argc       = 1;
            napi_value   argv[argc] = {args[0]};

            napi_value cons;
            status = napi_get_reference_value(env, constructor, &cons);
            assert(status == napi_ok);

            napi_value instance;
            status = napi_new_instance(env, cons, argc, argv, &instance);
            assert(status == napi_ok);

            return instance;
        }
    }
};

napi_value Open(napi_env env, napi_callback_info args)
{
    napi_value  greeting;
    napi_status status;

    status = napi_create_string_utf8(env, "hello", NAPI_AUTO_LENGTH, &greeting);
    if (status != napi_ok)
        return nullptr;

    return greeting;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)

} // namespace nodejs
} // namespace ot
