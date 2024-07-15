class database_model_resource : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("database_model_resource", "Node", false, false);
		
		//	TODO: add members and methods to database_model_resource.
		
		inst->CalculateSize();
		
		return inst;
	};
};

database_model_resource::Create();