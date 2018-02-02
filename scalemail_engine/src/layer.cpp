namespace ScaleMail
{
	//	Map height in pixels
static float gMaxLayers = 10.0f;
static float gMapHeight = 1024.0f;

//	============================================================================
float getLayerZ(int layer, float y) {
	return (layer + (y / gMapHeight)) / gMaxLayers;
}

//	============================================================================
void initializeLayers(float mapHeight) {
	gMapHeight = mapHeight;
}
}