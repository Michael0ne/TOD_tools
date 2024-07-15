class master_control_streamed_sound_Emitter : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("master_control_streamed_sound_Emitter", "Node", false, false);
		
		//	TODO: add members and methods to master_control_streamed_sound_Emitter.
		
		inst->CalculateSize();
		
		return inst;
	};
};

master_control_streamed_sound_Emitter::Create();