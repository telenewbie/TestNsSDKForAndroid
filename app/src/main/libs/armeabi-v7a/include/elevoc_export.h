#ifndef ELEVOC_EXPORT_H
#define ELEVOC_EXPORT_H

#if defined(_MSC_VER)
#define EV_IS_EXPORT __declspec(dllexport) 
#elif defined(__GNUC__)
#define EV_IS_EXPORT __attribute__((visibility("default"))) 
#else
#define EV_IS_EXPORT
#endif


#ifdef __cplusplus
extern "C" {
#endif

typedef void(*output_callback)(const char* out, int out_len, int out_type);

/** 
 * @brief 创建一个算法句柄
 * @return 算法句柄
*/
EV_IS_EXPORT void* elevoc_create_algo_manager();

/**
 * @brief 设置算法的采样率
 * @param handle 算法句柄
 * @param samplerate 采样率
 * @return 
*/
EV_IS_EXPORT void elevoc_set_samplerate(void* handle, int samplerate);

/**
 * @brief 设置算法的通道数
 * @param handle 算法句柄
 * @param channels 通道数
 * @return 
*/
EV_IS_EXPORT void elevoc_set_channels(void* handle, int channels);

/**
 * @brief 设置完参数后，初始化算法
 * @param handle 算法句柄
 * @return 
*/
EV_IS_EXPORT void elevoc_algo_manager_init(void* handle);

/**
 * @brief 设置算法执行结果回调
 * @param handle 算法句柄
 * @param callback 回调函数 typedef void(*output_callback)(const char* out, int out_len, int out_type);
 * @return
*/
EV_IS_EXPORT void elevoc_set_output_callback(void* handle, output_callback callback);

/**
 * @brief 传递音频数据给算法, 处理结果通过回调传回
 * @param handle 算法句柄
 * @param in 音频数据指针
 * @param in_len 音频数据长度
 * @return 
*/
EV_IS_EXPORT void elevoc_write_audio(void* handle, char* in, int in_len);


/**
 * @brief 传递音频数据给算法, 处理结果通过out传回，该接口与elevoc_write_audio互斥，对于同一个算法实例，只能调用其中一个接口
 * @param handle 算法句柄
 * @param in 输入数组
 * @param in_len in长度，不能超过一帧的长度
 * @param out 输出数组
 * @param out_len out长度
 * @return 大于0 正常，out长度， -1 输入长度不足一帧, 输入会被缓存起来， -2 输入长度超过一帧，直接返回，不会缓存,  -3 输出缓存不够，不足以存储一帧的数据
*/
EV_IS_EXPORT int elevoc_write_audio_sync(void* handle, const char* in, int in_len, char* out, int out_len);

/**
 * @brief 返回一帧输入数据的最大byte长度，需要在elevoc_algo_manager_init后调用
 * @param handle 算法句柄
 * @return 长度
*/
EV_IS_EXPORT int elevoc_get_max_input_bytes(void* handle);

/**
 * @brief 返回一帧输出数据的最大byte长度，需要在elevoc_algo_manager_init后调用
 * @param handle 算法句柄
 * @return 长度
*/
EV_IS_EXPORT int elevoc_get_max_output_bytes(void* handle);


/** 
* @brief  回收算法资源
* @param handle  算法句柄
* @return */
EV_IS_EXPORT void elevoc_destory_algo_manager(void* handle);


#ifdef __cplusplus
}
#endif

#endif // ELEVOC_EXPORT_H