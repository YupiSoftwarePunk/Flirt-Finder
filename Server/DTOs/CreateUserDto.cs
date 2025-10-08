
namespace Server.DTOs
{
    public class CreateUserDto
    {
        public string Username { get; internal set; }
        public string Bio { get; internal set; }
        public string Gender { get; internal set; }
        public DateTime? BirthDate { get; internal set; }
        public string PhotoUrl { get; internal set; }
        public object Password { get; internal set; }
    }
}
