class character_face_ctrl : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("character_face_ctrl", "Node", false, false);
		
		//	TODO: add members and methods to character_face_ctrl.
		
		inst->CalculateSize();
		
		return inst;
	};
};

character_face_ctrl::Create();