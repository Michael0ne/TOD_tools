class ClothTextureAnimate : public Scriptbaked
{
public:
	static Scriptbaked* Create()
	{
		static Scriptbaked* inst = new Scriptbaked("ClothTextureAnimate", "Node", false, false);
		
		//	TODO: add members and methods to ClothTextureAnimate.
		
		inst->CalculateSize();
		
		return inst;
	};
};

ClothTextureAnimate::Create();