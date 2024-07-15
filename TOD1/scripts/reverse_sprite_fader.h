class reverse_sprite_fader : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("reverse_sprite_fader", "Node", false, false);
		
		//	TODO: add members and methods to reverse_sprite_fader.
		
		inst->CalculateSize();
		
		return inst;
	};
};

reverse_sprite_fader::Create();