class vaporize_object_placeholder : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("vaporize_object_placeholder", "Node", false, false);
		
		//	TODO: add members and methods to vaporize_object_placeholder.
		
		inst->CalculateSize();
		
		return inst;
	};
};

vaporize_object_placeholder::Create();