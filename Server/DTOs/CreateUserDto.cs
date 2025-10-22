
namespace Server.DTOs
{
    public class CreateUserDto
    {
        public string Username { get; set; }
        public string Bio { get; set; }
        public string Gender { get; set; }
        public int Age { get; set; }
        public string PhotoUrl { get; set; }
        public string Password { get; set; }
        public string City { get; set; }
        public string Login { get; set; }
    }
}
