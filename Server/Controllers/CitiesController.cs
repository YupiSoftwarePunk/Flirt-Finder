using Microsoft.AspNetCore.Mvc;
using Server.Services.Interfaces;


namespace Server.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class CitiesController : ControllerBase
    {
        private readonly ICityService _cityService;

        public CitiesController(ICityService cityService)
        {
            _cityService = cityService;
        }

        [HttpGet("exists")]
        public async Task<IActionResult> CheckCityExists([FromQuery] string name)
        {
            if (string.IsNullOrWhiteSpace(name))
                return BadRequest("Название города не указано");

            bool exists = await _cityService.CityExistsAsync(name);
            return Ok(new { exists });
        }
    }
}
