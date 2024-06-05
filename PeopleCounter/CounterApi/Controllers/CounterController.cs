using CounterApi.Services;
using Microsoft.AspNetCore.Mvc;

namespace CounterApi.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class CounterController : Controller
    {
        private readonly ICounterService _counterService;
        public CounterController(ICounterService counterService)
        {
            _counterService = counterService;
        }

        [HttpGet(Name = "GetPeopleCount")]
        public ActionResult GetPeopleCount()
        {
            var count = _counterService.GetPeopleCount();
            if (count < 0)
            {
                return BadRequest();
            }

            return Ok(count);
        }
    }
}
