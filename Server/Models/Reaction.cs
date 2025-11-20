using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Server.Models
{
    [Table("likes_dislikes")]
    public class Reaction
    {
        [Key]
        [Column("id")]
        public int Id { get; set; }

        [Column("user_id")]
        public int UserId { get; set; }

        [Column("liked_by")]
        public int LikedBy { get; set; }

        [Column("reaction")]
        public int UserReaction { get; set; }



        [ForeignKey("UserId")]
        public User TargetUser { get; set; }

        [ForeignKey("LikedBy")]
        public User Liker { get; set; }
    }
}
