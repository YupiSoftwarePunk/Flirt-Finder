using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Server.Models
{
    [Table("photos")]
    public class Photo
    {
        [Key]
        [Column("id")]
        public int Id { get; set; }

        [Column("user_id")]
        public int UserId { get; set; }

        [Column("photo_path")]
        public string Url { get; set; }

        //public string ContentType { get; set; }


        [ForeignKey("UserId")]
        public User User { get; set; }
    }
}
