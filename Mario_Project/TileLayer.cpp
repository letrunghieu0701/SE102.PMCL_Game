#include "TileLayer.h"

void CTileLayer::Render()
{
	for (int i = 0; i < layer.size(); i++)
	{
		// Render từng sprite trong layer
		layer[i]->Render();
	}
}
