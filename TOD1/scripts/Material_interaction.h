class Material_interaction : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("Material_interaction", "Node", false, false);
		
		//	TODO: add members and methods to Material_interaction.
		
		inst->CalculateSize();
		
		return inst;
	};
};

Material_interaction::Create();