#ifndef IEVProcessTask_h
#define IEVProcessTask_h

#include <inttypes.h>
class IEVProcessTask {
	
public:

    /**
     * 设置采样率
     @param sampleRate 8000 or 16000 or 32000 or 48000
     */
    virtual void setSampleRate(int sampleRate) = 0;

	/**
	 * 设置channel数 默认是单通道
	 * @param channels (1 or 2)
	 */
	virtual void setChannels(int mic,int ref) = 0;

    /**
     * 参数设置完毕
     */
    virtual void prepared() = 0;
	/**
	开启处理task
	[en: Get the processed audio data from Speech Enhancement Module]
	@return 返回是否启动成功 [en: The returned value is True if it starts successfully, elsewise returned false]
	*/
	virtual bool startTask() = 0;
	/**
	写入音频数据
	[en: Send the unprocessed audio data to Speech Enhancement Module]
	@param audioData 音频pcm数组 [en: a pointer pointing to a array, which stores the audio data with pcm format]
	@param len 数组长度 [en: the size of the array]
	*/
	virtual void writeAudio(const char *audioData, size_t len) = 0;

	/**
	主动读取处理后结果接口
	[en: Get the processed data in active way]
	@param buffer 用于存放数的数组(外部需要保证足够长) [en: a array used to store the processed audio fetched from the engine ]
	@param len 读取的长度，如果读取长度超过当前长度，会读取当前队列全部数据。 [en: the maxium length of fetched data]
	@param isLast 是否是最后一组数据。 [en: whether the last batch of data or not]
	@return 实际读到的长度 [en: The actual length of data reading from the engine]
	*/
	virtual size_t readBuffer(char *buffer, size_t len, bool *isLast, bool isPaddingZero = false) = 0;
	/**
	停止降噪任务
	[en: Stop speech enhancement]
	*/
	virtual void stopTask(bool stopImmediately = true) = 0;

	/**
	启用/停用降噪
	[en:Continue/Pause denoising]
	* **/
	virtual void setEnableProcess(bool enable) = 0;

    /**
     * 设置network license 保存目录,用于缓存，避免重复进行网络请求
     */
    virtual void setNetworkLicenseSaveDir(const char * dir) = 0;

	/**
	* 获取当前的SDK版本号
	* [en: Get the sdk version]
	* @return 当前的SDK版本号
	*/
	virtual const char* libVer() = 0;

    /**
    * 设置参数
    * @param id enum {
                    PARAM_NS_SCALE = 0x100, //降噪系数  float,默认值:1.0，范围[0 - 2]
                    PARAM_NS_FLOOR_LOW_DB, //   float,默认值:-40，[-100 ~ 0 ]
                    PARAM_NS_FLOOR_HIGH_DB,//   float,默认值:-40，[-100 ~ 0 ]
                };
    * @param value [0 ~ 2.0] 推荐值：1.0
    */
    virtual void setParam(int id,float value) = 0;

    /**
     * 设置网络鉴权请求的回调，仅再发生网络请求时才会有回调
     */
    virtual void setAuthCallback(void(*msg)(const char *)) = 0;

};

extern "C" {
	__attribute__((visibility("default"))) IEVProcessTask *evNewTaskWithJavaVM(const char *appid, const char *secreatKey,const char * assert_dir, void *gs_jvm);
	__attribute__((visibility("default"))) void evDeleteTask(IEVProcessTask * pTask);
}

#endif
