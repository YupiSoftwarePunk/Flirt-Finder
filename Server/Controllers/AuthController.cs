using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Server.DTOs;
using Server.Services.Interfaces;

namespace Server.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class AuthController : ControllerBase
    {
        private readonly IAuthService _authService;

        public AuthController(IAuthService authService)
        {
            _authService = authService;
        }

        [HttpPost("register")]
        public async Task<IActionResult> Register(RegisterDto dto)
        {
            var result = await _authService.Register(dto);
            return result.Success ? Ok(result) : BadRequest(result.Message);
        }


        [HttpPost("login")]
        public async Task<IActionResult> Login(LoginDto dto)
        {
            var token = await _authService.Login(dto);
            return token != null ? Ok(new { token }) : Unauthorized();
        }
    }
}
