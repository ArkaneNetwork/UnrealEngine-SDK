using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.Logging;
using Venly;
using Venly.Companion;

namespace VenlyProviderService
{
    public static class Execute
    {
        [FunctionName("Execute")]
        public static async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Function, "get", "post", Route = null)] HttpRequest req,
            ILogger log)
        {
            //(optional) Here you can also configure the Endpoint Guard
            //Changing the default behavior of what endpoints are allowed/denied
            VenlyAPI.Backend.AllowEndpoint(VenlyAPI.Wallet.IDs.ExecuteCryptoTokenTransfer);
            VenlyAPI.Backend.AllowEndpoint(VenlyAPI.Wallet.IDs.ExecuteMultiTokenTransfer);
            VenlyAPI.Backend.AllowEndpoint(VenlyAPI.Wallet.IDs.ExecuteNativeTokenTransfer);
            //...

            return await VenlyAzure.HandleRequest(req);
        }
    }
}
