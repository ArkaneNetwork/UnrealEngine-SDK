using System.Threading.Tasks;
using Beamable.Server;
using Venly.Companion;

namespace Beamable.VenlyProviderService
{
	[Microservice("VenlyProviderService")]
	public class VenlyProviderService : VenlyMicroservice
	{
		[ClientCallable]
		public async Task<string> Execute(string request)
		{
			var response =  await HandleRequest(request);
			return response;
		}
	}
}
